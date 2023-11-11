#ifndef _SEM_LIST_H
#define _SEM_LIST_H

class KernelSem;

class SemList{
public:

	class SemNode {
		public:
			KernelSem* kernelSem;
			SemNode* next;

			SemNode(KernelSem* kernelSem,SemNode* next = 0) {
				this->kernelSem = kernelSem;
				this->next = next;
			}

			~SemNode() {}
	};

public:

	SemList(){
		first = 0;
		last = 0;
	}
	~SemList();

	static void update();

	int insert(KernelSem* kerSem);
	int remove(KernelSem* kerSem);

	KernelSem* getFirst();

	static SemNode* first;
	static SemNode* last;

};

extern SemList allSem;

#endif
