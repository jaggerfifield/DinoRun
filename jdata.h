struct Jdata{
	char* name;
	int type;
	int id;
	void* data;
	void* next;
	struct Jdata* tail;
};

struct Jdata* init(char*, int, int, void*, void*);
struct Jdata* add_data(struct Jdata*, int, int, int, int, char*, char*);
void jdata_free(struct Jdata*);
