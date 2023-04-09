struct Jdata{
	int id;
	int size;
	char* name;
	void* data;
	void* next;
	struct Jdata* tail;
	int type;
};

struct Jdata* init(int);
struct Jdata* add_data(struct Jdata*, int, char*, int, int, int, const char*, char*);
void* find_node(struct Jdata*, int);
void jdata_free(struct Jdata*);
