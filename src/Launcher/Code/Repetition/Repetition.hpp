#ifndef LAUNCHER_REPETITION_HPP_
#define LAUNCHER_REPETITION_HPP_

namespace aff3ct
{
namespace launcher
{
template <class L, typename B = int, typename R = float, typename Q = R>
class Repetition : public L
{
public:
	Repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Repetition();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void print_header();
};
}
}

#include "Repetition.hxx"

#endif /* LAUNCHER_REPETITION_HPP_ */
