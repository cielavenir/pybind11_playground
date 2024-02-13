#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

extern "C" {
#include <stdlib.h>
#include <stdio.h>
}

// Be careful about PY_MAJOR_VERSION and PYBIND11_VERSION_MAJOR (the order)

namespace pybind11 { namespace detail {

template <> struct type_caster<__uint128_t> {
public:
    PYBIND11_TYPE_CASTER(__uint128_t, const_name("__uint128_t"));
    bool load(handle src, bool convert) {
        if (isinstance<int_>(src)) {
            if(src >= (pybind11::cast(1)<<pybind11::cast(128))){
                return false;
            }
            value = __uint128_t( ((__uint128_t)(pybind11::cast<uint64_t>(src>>pybind11::cast(64)))<<64) | pybind11::cast<uint64_t>(src&pybind11::cast(0xffffffffffffffffULL)) );
            return true;
        }
        return false;
    }

    static handle cast(const __uint128_t &src, return_value_policy policy, handle parent) {
        return ((pybind11::cast((uint64_t)(src>>64)) << pybind11::cast(64)) | pybind11::cast((uint64_t)(src & 0xffffffffffffffffULL))).inc_ref();
        //return reinterpret_borrow<object>((pybind11::cast((uint64_t)(src>>64)) << pybind11::cast(64)) | pybind11::cast((uint64_t)(src & 0xffffffffffffffffULL)));
    }
};

}}

namespace py = pybind11;
using namespace pybind11::literals;

class toyclass{
public:
    // to confirm that GIL is not released
    void sleep(int duration){
        ::sleep(duration);
    }

    __uint128_t copyUint128(__uint128_t val){
        return val;
    }

    __uint128_t generateUint128(){
        __uint128_t val = ((__uint128_t)(0x1234567812345678)<<64) | 0x90abcdef90abcdef;
        return val;
    }

    // to confirm that reference count increases with py::cast but not py::reinterpret_steal
    void test_int_cast(){
        for(int i=0;i<10000;i++){
            py::object o = py::cast<py::object>(
#if PY_MAJOR_VERSION >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
        }
            py::object o = py::cast<py::object>(
#if PY_MAJOR_VERSION >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
            printf("[test_int_cast] %d\n", o.ref_count());
    }
    void test_int_steal(){
        for(int i=0;i<10000;i++){
            py::object o = py::reinterpret_steal<py::object>(
#if PY_MAJOR_VERSION >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
        }
            py::object o = py::reinterpret_steal<py::object>(
#if PY_MAJOR_VERSION >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
            printf("[test_int_steal] %d\n", o.ref_count());
    }

    // to check pybind11_stubgen signature
    // https://github.com/sizmailov/pybind11-stubgen/issues/96
    void test_show_2_ints(std::array<int, 2> ints){
        printf("[test_show_2_ints] %d, %d\n", ints[0], ints[1]);
    }

    // to check overload stubgen order
    // https://github.com/sizmailov/pybind11-stubgen/pull/98
    void test_overload(py::str o){
        puts("[test_overload] str");
    }
    void test_overload(py::object o){
        puts("[test_overload] object");
    }
};

PYBIND11_MODULE(pybind11_playground, m){
    void (toyclass::*test_overload1)(py::str) = &toyclass::test_overload;
    void (toyclass::*test_overload2)(py::object) = &toyclass::test_overload;

    py::class_<toyclass, std::shared_ptr<toyclass> >(m, "toyclass")
    .def(py::init<>())
    .def("sleep", &toyclass::sleep,
        "duration"_a
    )
    .def("copyUint128", &toyclass::copyUint128, "arg"_a)
    .def("generateUint128", &toyclass::generateUint128)
    .def("test_int_cast", &toyclass::test_int_cast)
    .def("test_int_steal", &toyclass::test_int_steal)
    .def("test_show_2_ints", &toyclass::test_show_2_ints,
        "ints"_a
    )
    .def("test_overload", test_overload1)
    .def("test_overload", test_overload2)
    ;
}
