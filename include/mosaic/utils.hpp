//
// Created by yhkim on 2026. 2. 17.
//

#ifndef MOSAIC_CORE_UTILS_HPP
#define MOSAIC_CORE_UTILS_HPP

#include <memory>

template <typename T>
using impl_ptr = std::shared_ptr<T>;
template <typename T>
class CheshireCat {
  public:
    explicit CheshireCat(impl_ptr<T> impl) : mImpl(std::move(impl)) {}
    template <typename... Args>
    explicit CheshireCat(Args... args) : mImpl(std::make_shared<T>(std::forward<Args>(args)...)) {}
    CheshireCat(CheshireCat<T>&& cc) noexcept {
        *this = std::move(cc);
    }
    CheshireCat(const CheshireCat<T>&) = delete;

    virtual ~CheshireCat() = default;

    CheshireCat& operator=(CheshireCat<T>&& cc) noexcept {
        mImpl = std::move(cc.mImpl);
        return *this;
    };
    CheshireCat& operator=(const CheshireCat<T>&) = delete;

  protected:
    impl_ptr<T> impl() {
        return mImpl;
    }
    impl_ptr<const T> impl() const {
        return mImpl;
    }

  private:
    impl_ptr<T> mImpl;
};

#endif  // MOSAIC_CORE_UTILS_HPP
