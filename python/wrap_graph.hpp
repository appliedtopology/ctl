// Creates a Python class for an `Simplex_boundary`. 
struct Graph_wrapper {
  typedef ctl::Nbhd_graph<> Graph; 
  typedef typename boost::graph_traits<Graph> graph_traits;
  typedef typename graph_traits::vertex_iterator vertex_iterator;
  typedef typename boost::graph_traits< Graph>::vertex_descriptor vertex_descriptor; 
  typedef typename boost::property_map< Graph,
      				  boost::vertex_name_t>::type name_map_t;
  typedef typename boost::property_traits< name_map_t>::value_type  vertex_name_t;
  typedef std::unordered_map< vertex_name_t, vertex_descriptor> Name_to_descriptor_map;
  Graph_wrapper() {}
  Graph_wrapper( std::size_t n):  graph( n) {}
  
  Graph graph = Graph();
  name_map_t name_map = boost::get( boost::vertex_name, graph);
  Name_to_descriptor_map descriptor;
  void insert_vertex(){
    std::size_t i = boost::num_vertices( graph);
    vertex_descriptor v_descriptor = boost::add_vertex( graph);
    name_map[ v_descriptor] = i;
    descriptor.emplace( i, v_descriptor);
  }

  void insert_edge( std::size_t i, std::size_t j, double w){
      auto vi = descriptor[ i];
      auto vj = descriptor[ j];
      boost::add_edge(vi, vj, w, graph);

  }
  std::size_t size() const {
  	return boost::num_vertices( graph);
  }
};

std::ostream& 
operator<<(std::ostream& out, const Graph_wrapper& g){
	out << g.graph;
	return out;
}

void wrap_graph(py::module &mod){
   using s = Graph_wrapper;
   py::class_<s>(mod, std::string("Nbhd_graph").c_str())
    //Default no-args constructor
    .def(py::init())
    .def("__init__", [](s & instance, std::size_t n){
    	new (&instance) s( n); 
    })
    .def("insert_vertex", [](s& graph){
	graph.insert_vertex();
        }, "insert vertex")
    .def("insert_edge", [](s& graph, std::size_t i, std::size_t j, double w){
    	graph.insert_edge( i, j, w);
	}, "insert simplex and its boundary")
    .def("__len__", &s::size)
    .def("__iter__", [](const s &g) {
	auto pair = boost::vertices(g.graph);
        return py::make_iterator(pair.first, pair.second);
    }, py::keep_alive<0, 1>())
    .def("__str__", stream_to_string<s>());
}
