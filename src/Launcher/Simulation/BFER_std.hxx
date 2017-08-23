#include "Simulation/BFER/Standard/BFER_std.hpp"

#include <thread>
#include <string>
#include <iostream>

#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "BFER_std.hpp"

namespace aff3ct
{
namespace launcher
{
template <class C, typename B, typename R, typename Q, int CRC, int ITL>
BFER_std<C,B,R,Q,CRC,ITL>
::BFER_std(const int argc, const char **argv, std::ostream &stream)
: Launcher(argc, argv, params, stream)
{
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
BFER_std<C,B,R,Q,CRC,ITL>
::~BFER_std()
{
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
::build_args()
{
	Launcher::build_args();

	factory::BFER_std     ::build_args(this->req_args, this->opt_args);
	factory::Source       ::build_args(this->req_args, this->opt_args);
	factory::CRC          ::build_args(this->req_args, this->opt_args);
	factory::Modem        ::build_args(this->req_args, this->opt_args);
	factory::Channel      ::build_args(this->req_args, this->opt_args);
	if (std::is_integral<Q>())
		factory::Quantizer::build_args(this->req_args, this->opt_args);
	factory::Monitor      ::build_args(this->req_args, this->opt_args);
	factory::Terminal_BFER::build_args(this->req_args, this->opt_args);

	if (this->req_args.find({"enc-info-bits", "K"}) != this->req_args.end() ||
	    this->req_args.find({"pct-info-bits", "K"}) != this->req_args.end())
		this->req_args.erase({"src-info-bits", "K"});
	this->opt_args.erase({"src-seed",      "S"});
	this->req_args.erase({"crc-info-bits", "K"});
	this->opt_args.erase({"crc-fra",       "F"});
	this->req_args.erase({"mdm-fra-size",  "N"});
	this->opt_args.erase({"mdm-fra",       "F"});
	this->opt_args.erase({"mdm-sigma"        });
	this->req_args.erase({"chn-fra-size",  "N"});
	this->opt_args.erase({"chn-fra",       "F"});
	this->opt_args.erase({"chn-sigma"         });
	this->opt_args.erase({"chn-seed",      "S"});
	this->opt_args.erase({"chn-add-users"     });
	this->opt_args.erase({"chn-complex"       });
	this->req_args.erase({"qnt-size",      "N"});
	this->opt_args.erase({"qnt-fra",       "F"});
	this->opt_args.erase({"qnt-sigma"         });
	this->req_args.erase({"mnt-size",      "K"});
	this->opt_args.erase({"mnt-fra",       "F"});
	this->req_args.erase({"ter-info-bits", "K"});
	this->opt_args.erase({"ter-cw-size",   "N"});
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
::store_args()
{
	Launcher::store_args();

	factory::BFER_std::store_args(this->ar.get_args(), params);

	params.src.seed = params.local_seed;

	factory::Source::store_args(this->ar.get_args(), params.src);

	auto K    = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params.src.K : params.cdc.K;
	auto N    = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params.src.K : params.cdc.N;
	auto N_cw = this->req_args.find({"src-info-bits", "K"}) != this->req_args.end() ? params.src.K : params.cdc.N_cw;

	factory::CRC::store_args(this->ar.get_args(), params.crc);

	params.crc.K = K - params.crc.size;
	params.src.K = params.src.K == 0 ? params.crc.K : params.src.K;
	params.mdm.N = N;

	factory::Modem::store_args(this->ar.get_args(), params.mdm);

	params.chn.N         = params.mdm.N_mod;
	params.chn.complex   = params.mdm.complex;
	params.chn.add_users = params.mdm.type == "SCMA";
	params.chn.seed      = params.local_seed;

	factory::Channel::store_args(this->ar.get_args(), params.chn);

	params.qnt.size = params.mdm.N;

	if (std::is_integral<Q>())
		factory::Quantizer::store_args(this->ar.get_args(), params.qnt);

	params.mnt.size = params.coded_monitoring ? N_cw : params.src.K;

	factory::Monitor::store_args(this->ar.get_args(), params.mnt);

	factory::Terminal_BFER::store_args(this->ar.get_args(), params.ter);

	if (!std::is_integral<Q>())
		params.qnt.type = "NO";

	if (params.coset)
		params.cdc.enc.type = "COSET";
	else if (params.cdc.enc.type == "COSET")
		params.coset = true;

	if (params.src.type == "AZCW" || params.cdc.enc.type == "AZCW")
	{
		params.src    .type = "AZCW";
		params.cdc.enc.type = "AZCW";
	}

	params.cdc.enc.seed = params.local_seed;

	params.crc.n_frames = params.src.n_frames;
	params.mdm.n_frames = params.src.n_frames;
	params.chn.n_frames = params.src.n_frames;
	params.qnt.n_frames = params.src.n_frames;
	params.mnt.n_frames = params.src.n_frames;
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
::group_args()
{
	Launcher::group_args();

	this->arg_group.push_back({factory::Simulation ::prefix, factory::Simulation ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Source     ::prefix, factory::Source     ::name + " parameter(s)"});
	this->arg_group.push_back({factory::CRC        ::prefix, factory::CRC        ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Encoder    ::prefix, factory::Encoder    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Interleaver::prefix, factory::Interleaver::name + " parameter(s)"});
	this->arg_group.push_back({factory::Puncturer  ::prefix, factory::Puncturer  ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Modem      ::prefix, factory::Modem      ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Channel    ::prefix, factory::Channel    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Quantizer  ::prefix, factory::Quantizer  ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Decoder    ::prefix, factory::Decoder    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Monitor    ::prefix, factory::Monitor    ::name + " parameter(s)"});
	this->arg_group.push_back({factory::Terminal   ::prefix, factory::Terminal   ::name + " parameter(s)"});
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
void BFER_std<C,B,R,Q,CRC,ITL>
::print_header()
{
	factory::BFER_std     ::make_header(this->pl_sim, params,     false);
	factory::Source       ::make_header(this->pl_src, params.src, false);
	factory::CRC          ::make_header(this->pl_crc, params.crc, false);
	factory::Modem        ::make_header(this->pl_mdm, params.mdm, false);
	factory::Channel      ::make_header(this->pl_chn, params.chn, false);
	if (std::is_integral<Q>())
		factory::Quantizer::make_header(this->pl_qnt, params.qnt, false);
	factory::Monitor      ::make_header(this->pl_mnt, params.mnt, false);
	factory::Terminal_BFER::make_header(this->pl_ter, params.ter, false);

	const auto code_rate = (float)params.cdc.K / (float)params.cdc.N;

	this->pl_cde.push_back(std::make_pair("Type",                                params.cde_type));
	this->pl_cde.push_back(std::make_pair("Info. bits (K)",       std::to_string(params.cdc.K   )));
	this->pl_cde.push_back(std::make_pair("Codeword size (N_cw)", std::to_string(params.cdc.N_cw)));
	this->pl_cde.push_back(std::make_pair("Frame size (N)",       std::to_string(params.cdc.N   )));
	this->pl_cde.push_back(std::make_pair("Code rate",            std::to_string(code_rate      )));

	Launcher::print_header();
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
simulation::Simulation* BFER_std<C,B,R,Q,CRC,ITL>
::build_simu()
{
	return factory::BFER_std::build<C,B,R,Q,CRC,ITL>(params);
}
}
}
