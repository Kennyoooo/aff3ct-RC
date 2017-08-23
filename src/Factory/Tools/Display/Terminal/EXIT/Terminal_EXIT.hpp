#ifndef FACTORY_TERMINAL_EXIT_HPP_
#define FACTORY_TERMINAL_EXIT_HPP_

#include <string>
#include <chrono>

#include "Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace factory
{
struct Terminal_EXIT : Terminal
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Terminal::parameters
	{
		virtual ~parameters() {}

		tools::Terminal_EXIT* build(const int &cur_t, const int &trials, const double &I_A, const double &I_E) const;

		int   N           = 0;
		float snr         = -1.f;
		float sig_a       = -1.f;

		std::string type  = "STD";
	};

	static tools::Terminal_EXIT* build(const parameters &params, const int &cur_t, const int &trials,
	                                   const double &I_A, const double &I_E);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_ter, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_TERMINAL_EXIT_HPP_ */
