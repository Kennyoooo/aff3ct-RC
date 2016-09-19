#include "Module/Quantizer/Standard/Quantizer_standard.hpp"
#include "Module/Quantizer/Fast/Quantizer_fast.hpp"
#include "Module/Quantizer/Tricky/Quantizer_tricky.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"

#include "Factory_quantizer.hpp"

template <typename R, typename Q>
Quantizer<R,Q>* Factory_quantizer<R,Q>
::build(const parameters &params, const R& sigma, const int size)
{
	Quantizer<R,Q> *quantizer = nullptr;

	// build the quantizer
	if (typeid(R) == typeid(Q))
		quantizer = new Quantizer_NO<R,Q>(size);
	else
	{
		if (params.quantizer.type == "STD")
			quantizer = new Quantizer_standard<R,Q>(size, params.quantizer.n_decimals, params.quantizer.n_bits, 1);
		else if (params.quantizer.type == "STD_FAST")
			quantizer = new Quantizer_fast<R,Q>(size, params.quantizer.n_decimals, params.quantizer.n_bits, 1);
		else if (params.quantizer.type == "TRICKY")
		{
			if (params.quantizer.range == 0.f)
				quantizer = new Quantizer_tricky<R,Q>(size, (short)params.quantizer.n_bits, sigma, 1); // auto mode
			else
				quantizer = new Quantizer_tricky<R,Q>(size, (R)params.quantizer.range, params.quantizer.n_bits, sigma, 1);
		}
	}

	return quantizer;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_quantizer<R_8,Q_8>;
template struct Factory_quantizer<R_16,Q_16>;
template struct Factory_quantizer<R_32,Q_32>;
template struct Factory_quantizer<R_64,Q_64>;
#else
template struct Factory_quantizer<R,Q>;
#endif
// ==================================================================================== explicit template instantiation
