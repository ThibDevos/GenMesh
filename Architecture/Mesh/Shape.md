
*Shape describes the topology of a shape: the dimension (D), the number of vertices, edges, ...*
```cpp
template<size_t D>
struct shape
{
	private:
		int nb_vertices;
		int nb_edges;
		int nb_faces;
		
	public:
		int get_nb_vertives();
		int get_nb_edges();
		int get_nb_faces();
		int get_nb_facets(); //facet is the shape of dim D-1
	 	
};
```

From there, we can define using macro simplices, hypercubes, prism
```cpp
struct vertex : shape<2>
{

}	

struct edge : shape<2>
{

}	

template<size_t D> //D> 1
struct simplex<D> : Shape<D>
{

}	

template<size_t D> //D> 1
struct hypercube<D> : Shape<D>
{

}


struct prism<D> : Shape<D>
{

}

enum ShapeType {vertex, edge, simplex, hypercube, prism};
```
