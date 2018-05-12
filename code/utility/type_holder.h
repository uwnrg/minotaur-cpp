#ifndef MINOTAUR_CPP_TYPE_HOLDER_H
#define MINOTAUR_CPP_TYPE_HOLDER_H

template<typename val_t>
struct type_holder {

    explicit type_holder(val_t &value) :
        m_value(value) {}

    val_t &operator()() const {return m_value; }

    operator val_t&() const { return m_value; }

private:
    val_t &m_value;
};

#endif //MINOTAUR_CPP_TYPE_HOLDER_H
