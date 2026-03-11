*The mesh is the union of a [[Topology]] and a [[Geometry]]*

```cpp
template<size_t D, size_t G>
class Mesh
{
	using entity_list<D> = std::vector<Entity<D,G,Mesh>;


	T<D> topo;
	Geo geo; //will contain coordinates
	entity_list<0> vertices;
	entity_list<1> edges;
	entity_list<2> faces;
	entity_list<D> cells;
};
```