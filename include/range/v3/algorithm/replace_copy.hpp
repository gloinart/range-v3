/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_REPLACE_COPY_HPP
#define RANGES_V3_ALGORITHM_REPLACE_COPY_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/meta.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \ingroup group-concepts
        template<typename I, typename O, typename T0, typename T1, typename P = ident>
        using ReplaceCopyable = meta::fast_and<
            InputIterator<I>,
            WeakOutputIterator<O, T1>,
            IndirectlyCopyable<I, O>,
            IndirectInvokableRelation<equal_to, Project<I, P>, T0 const *>>;

        /// \addtogroup group-algorithms
        /// @{
        struct replace_copy_fn
        {
            template<typename I, typename S, typename O, typename T0, typename T1, typename P = ident,
                CONCEPT_REQUIRES_(ReplaceCopyable<I, O, T0, T1, P>() && IteratorRange<I, S>())>
            std::pair<I, O> operator()(I begin, S end, O out, T0 const & old_value, T1 const & new_value, P proj_ = {}) const
            {
                auto &&proj = invokable(proj_);
                for(; begin != end; ++begin, ++out)
                {
                    auto &&x = *begin;
                    if(proj(x) == old_value)
                        *out = new_value;
                    else
                        *out = (decltype(x) &&) x;
                }
                return {begin, out};
            }

            template<typename Rng, typename O, typename T0, typename T1, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(ReplaceCopyable<I, O, T0, T1, P>() && Iterable<Rng &>())>
            std::pair<I, O> operator()(Rng & rng, O out, T0 const & old_value, T1 const & new_value, P proj = {}) const
            {
                return (*this)(begin(rng), end(rng), std::move(out), old_value, new_value, std::move(proj));
            }
        };

        /// \sa `replace_copy_fn`
        /// \ingroup group-algorithms
        namespace
        {
            constexpr auto&& replace_copy = static_const<replace_copy_fn>::value;
        }

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
