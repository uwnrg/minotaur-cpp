#ifndef MINOTAUR_CPP_WEAK_REF_H
#define MINOTAUR_CPP_WEAK_REF_H

template<typename ref_t>
class weak_ref {
public:
    explicit weak_ref(ref_t *ptr) :
        m_ptr(ptr) {}

    weak_ref(const weak_ref<ref_t> &ref) :
        m_ptr(ref.get()) {}

    ref_t *get() const { return m_ptr; }

    ref_t &operator*() const { return *m_ptr; }

    ref_t *operator->() const { return m_ptr; }

    operator ref_t *weak_ref<ref_t>::*() const { return m_ptr == nullptr ? 0 : &weak_ref<ref_t>::m_ptr; }

private:
    ref_t *m_ptr;
};

template<typename ref_t>
weak_ref<ref_t> as_weak(ref_t *ptr) {
    return weak_ref<ref_t>(ptr);
}

#endif //MINOTAUR_CPP_WEAK_REF_H
