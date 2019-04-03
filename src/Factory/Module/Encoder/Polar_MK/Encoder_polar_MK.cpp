#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Module/Encoder/Polar_MK/Encoder_polar_MK.hpp"
#include "Module/Encoder/Polar_MK/Encoder_polar_MK_sys.hpp"

#include "Encoder_polar_MK.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar_MK_name   = "Encoder Polar MK";
const std::string aff3ct::factory::Encoder_polar_MK_prefix = "enc";

Encoder_polar_MK::parameters
::parameters(const std::string &prefix)
: Encoder::parameters(Encoder_polar_MK_name, prefix)
{
	this->type = "POLAR_MK";
	this->systematic = false;
}

Encoder_polar_MK::parameters* Encoder_polar_MK::parameters
::clone() const
{
	return new Encoder_polar_MK::parameters(*this);
}

struct sub_same_length
{
	template <typename T>
	static void check(const std::vector<T>& v)
	{
		for(unsigned i = 0; i < v.size(); i++)
			if (v[i].size() != v[0].size())
				throw std::runtime_error("all elements do not have the same length");
	}
};

void Encoder_polar_MK::parameters
::get_description(tools::Argument_map_info &args) const
{
	Encoder::parameters::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_polar_MK::parameters::";

	tools::add_options(args.at({p+"-type"}), 0, "POLAR_MK");

	tools::add_arg(args, p, class_name+"p+sys",
		tools::None());
}

void Encoder_polar_MK::parameters
::store(const tools::Argument_map_value &vals)
{
	Encoder::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-sys"})) this->systematic = true;
}

void Encoder_polar_MK::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Encoder::parameters::get_headers(headers, full);

	// auto p = this->get_prefix();
}

template <typename B>
module::Encoder_polar_MK<B>* Encoder_polar_MK::parameters
::build(const tools::Polar_code &code, const std::vector<bool> &frozen_bits) const
{
	if (this->type == "POLAR_MK")
	{
		if (this->systematic) return new module::Encoder_polar_MK_sys<B>(this->K, this->N_cw, code, frozen_bits, this->n_frames);
		else                  return new module::Encoder_polar_MK    <B>(this->K, this->N_cw, code, frozen_bits, this->n_frames);
	}

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename B>
module::Encoder_polar_MK<B>* Encoder_polar_MK
::build(const parameters& params, const tools::Polar_code &code, const std::vector<bool> &frozen_bits)
{
	return params.template build<B>(code, frozen_bits);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_polar_MK<B_8 >* aff3ct::factory::Encoder_polar_MK::parameters::build<B_8 >(const tools::Polar_code&, const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_MK<B_16>* aff3ct::factory::Encoder_polar_MK::parameters::build<B_16>(const tools::Polar_code&, const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_MK<B_32>* aff3ct::factory::Encoder_polar_MK::parameters::build<B_32>(const tools::Polar_code&, const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_MK<B_64>* aff3ct::factory::Encoder_polar_MK::parameters::build<B_64>(const tools::Polar_code&, const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_MK<B_8 >* aff3ct::factory::Encoder_polar_MK::build<B_8 >(const aff3ct::factory::Encoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&);
template aff3ct::module::Encoder_polar_MK<B_16>* aff3ct::factory::Encoder_polar_MK::build<B_16>(const aff3ct::factory::Encoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&);
template aff3ct::module::Encoder_polar_MK<B_32>* aff3ct::factory::Encoder_polar_MK::build<B_32>(const aff3ct::factory::Encoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&);
template aff3ct::module::Encoder_polar_MK<B_64>* aff3ct::factory::Encoder_polar_MK::build<B_64>(const aff3ct::factory::Encoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&);
#else
template aff3ct::module::Encoder_polar_MK<B>* aff3ct::factory::Encoder_polar_MK::parameters::build<B>(const tools::Polar_code&, const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_MK<B>* aff3ct::factory::Encoder_polar_MK::build<B>(const aff3ct::factory::Encoder_polar_MK::parameters&, const tools::Polar_code&, const std::vector<bool>&);
#endif
// ==================================================================================== explicit template instantiation
