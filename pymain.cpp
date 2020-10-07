#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "ffcache.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ffcache, m) {
    py::class_<FirefoxCache>(m, "FirefoxCache")
        .def(py::init<string>(), py::arg("cache2_dir"))
        .def("keys", &FirefoxCache::keys)
        .def_readonly("records", &FirefoxCache::records)
        .def("find_save", &FirefoxCache::find_save, py::arg("key"), py::arg("path"))
        .def("find", &FirefoxCache::find, py::arg("key"));

    py::class_<FirefoxCacheEntry>(m, "FirefoxCacheEntry")
        .def_readonly("key", &FirefoxCacheEntry::key)
        .def("get_header", &FirefoxCacheEntry::get_header)
        .def("get_data",
             [](const FirefoxCacheEntry& self) {
                 auto v = self.get_data();
                 if (v->size() > 0) {
                     return py::bytes(&v->at(0), v->size());
                 } else {
                     return py::bytes();
                 }
             })
        .def("save", &FirefoxCacheEntry::save, py::arg("key"));

    py::class_<HttpHeader>(m, "HttpHeader")
        .def_readonly("status_code", &HttpHeader::status_code)
        .def_readonly("status_source", &HttpHeader::status_source)
        .def_readonly("protocol", &HttpHeader::protocol)
        .def_readonly("headers", &HttpHeader::headers);
}
