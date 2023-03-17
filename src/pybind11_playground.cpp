#include <pybind11/pybind11.h>

extern "C" {
#include <stdlib.h>
#include <stdio.h>
}

namespace py = pybind11;
using namespace pybind11::literals;

class toyclass{
public:
    void sleep(int duration){
        ::sleep(duration);
    }
    void test_int1(){
        for(int i=0;i<10000;i++){
            py::object o = py::cast<py::object>(
#if PY_VERSION_MAJOR >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
        }
            py::object o = py::cast<py::object>(
#if PY_VERSION_MAJOR >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
            printf("%d\n", o.ref_count());
    }
    void test_int2(){
        for(int i=0;i<10000;i++){
            py::object o = py::reinterpret_steal<py::object>(
#if PY_VERSION_MAJOR >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
        }
            py::object o = py::reinterpret_steal<py::object>(
#if PY_VERSION_MAJOR >= 3
                PyLong_FromLong
#else
                PyInt_FromLong
#endif
                (10)
            );
            printf("%d\n", o.ref_count());
    }
};

PYBIND11_MODULE(pybind11_playground, m){
    py::class_<toyclass, std::shared_ptr<toyclass> >(m, "toyclass")
    .def(py::init<>())
    .def("sleep", &toyclass::sleep,
     "duration"_a
    )
    .def("test_int1", &toyclass::test_int1)
    .def("test_int2", &toyclass::test_int2)
    ;
}
