struct Jdata{
	char* name;
	int type;
	int id;
	void* data;
	void* next;
	struct Jdata* tail;
};

struct Jdata* init(char*, int, int, void*, void*);
struct Jdata* add_data(struct Jdata*, int, int, int, int, const char*, char*);
void* find_node(struct Jdata*, int);
void jdata_free(struct Jdata*);
