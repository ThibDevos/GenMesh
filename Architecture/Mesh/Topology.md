*The topology of the mesh reprensents the different relations between the entities: adjacence relation and incidence relation*

```cpp
template<size_t D>
struct topology
{
	private:
		
		
		connectivity<0,1> C_VE;
		connectivity<0,2> C_VF;
		connectivity<0,D> C_VC;
		....
};
```