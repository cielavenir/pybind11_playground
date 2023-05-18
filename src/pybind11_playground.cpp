#include <pybind11/pybind11.h>

extern "C" {
#include <stdlib.h>
#include <stdio.h>
}

// Be careful about PY_MAJOR_VERSION and PYBIND11_VERSION_MAJOR (the order)

namespace py = pybind11;
using namespace pybind11::literals;

class toyclass{
public:
    // to confirm that GIL is not released
    void sleep(int duration){
        ::sleep(duration);
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
    void show_2_ints(std::array<int, 2> ints){
        printf("[show_2_ints] %d, %d\n", ints[0], ints[1]);
    }
};

PYBIND11_MODULE(pybind11_playground, m){
    py::class_<toyclass, std::shared_ptr<toyclass> >(m, "toyclass")
    .def(py::init<>())
    .def("sleep", &toyclass::sleep,
        "duration"_a
    )
    .def("test_int_cast", &toyclass::test_int_cast)
    .def("test_int_steal", &toyclass::test_int_steal)
    .def("show_2_ints", &toyclass::show_2_ints,
        "ints"_a
    )
    ;
}
