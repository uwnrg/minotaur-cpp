#ifndef MINOTAUR_CPP_WEAK_REF_H
#define MINOTAUR_CPP_WEAK_REF_H

template<typename ref_t>
class weak_ref {
public:
    weak_ref(ref_t *ptr) :
        m_ptr(ptr) {}

    ref_t *get() { return m_ptr; }

    const ref_t *get() const { return m_ptr; }

    ref_t &operator*() { return *m_ptr; }

    ref_t *operator->() { return m_ptr; }

    const ref_t &operator*() const { return *m_ptr; }

    const ref_t *operator->() const { return m_ptr; }

private:
    ref_t *m_ptr;
};

#endif //MINOTAUR_CPP_WEAK_REF_H
