#include <cilk/reducer.h>

template<typename T>
class elementwise_reducer {
    struct View {
        T* array;
        View( size_t k ) : array( new T[k] ) {}
        ~View() {delete[] array;}
    };

    struct Monoid: cilk::monoid_base<View> {
        const size_t k;
        void identity(View* p) const {new(p) View(k);}
        void reduce(View* left, View* right) const {
            left->array[0:k] += right->array[0:k];
        }
        Monoid( size_t k_ ) : k(k_) {}
    };
    cilk::reducer<Monoid> impl;
public:
    elementwise_reducer( size_t k ) : impl(Monoid(k), k) {}
#if __linux__||__APPLE__
    void clear() {[]( T& x) {x.clear();}(impl.view().array[0:impl.monoid().k]);}
#else
    void clear() {impl.view().array[0:impl.monoid().k].clear();}
#endif
    T* get_array() {return impl.view().array;}
    operator sum_and_count*() {return get_array();}
};
