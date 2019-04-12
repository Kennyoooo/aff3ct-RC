#ifndef DECODER_POLAR_MK_ASCL_NAIVE_CA_SYS
#define DECODER_POLAR_MK_ASCL_NAIVE_CA_SYS

#include "../SC/Decoder_polar_MK_SC_naive_sys.hpp"
#include "../SCL/CRC/Decoder_polar_MK_SCL_naive_CA_sys.hpp"

#include "Module/CRC/CRC.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_polar_MK_ASCL_naive_CA_sys : public Decoder_polar_MK_SCL_naive_CA_sys<B,R>
{
private:
	Decoder_polar_MK_SC_naive_sys<B,R> sc_decoder;

public:
	Decoder_polar_MK_ASCL_naive_CA_sys(const int& K,
	                                   const int& N,
	                                   const int& L,
	                                   const tools::Polar_code& code,
	                                   const std::vector<bool>& frozen_bits,
	                                   CRC<B>& crc,
	                                   const int n_frames = 1);

	virtual ~Decoder_polar_MK_ASCL_naive_CA_sys() = default;

	virtual void notify_frozenbits_update();

protected:
	void _decode        (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#endif /* DECODER_POLAR_MK_ASCL_NAIVE_CA_SYS */
