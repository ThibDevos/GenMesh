
*An entity represents an entity of the [[mesh]]. It is a [[Shape]] with coordinates and incidence relations (entities of lower dimensions forming this entity) and adjacency relations (entities of same topological dimension that share a common entity of lower topo dim).
It has two dimensions: D and G which are the topological entities (the one from the shape) and geometrical (the dimension in which space it lives)*

```cpp

template< size_t D, size_t G, typename Mesh>
struct Entity
{
	private:
		ShapeType S;
		int index; //Index of the entity in the mesh
		Coords* Coordinates; // Pointer to the coordinates stored in the Mesh
		Mesh* mesh;
		
	public:
		auto get_incidence(int d); //return the incident entities of topo dim d
		auto get_adjency(); //return the direct neighbours (from the adjacency graphs defined in the topology)
		
		/* All the geometrical informations 
		- measure
		- normal
		- barycentre
		  ...
		  */
};
```