#pragma once

class RCObject{
public:
	RCObject() : refCount(0), shareable(true){}
	RCObject(const RCObject& obj) : refCount(0), shareable(true){}
	RCObject& operator=(const RCObject &obj){return *this;}
	virtual ~RCObject() {}
	void addReference(){ ++ refCount;}
	void removeReference() { if(--refCount == 0) delete this; }
	void markUnShareable() { shareable = false;}
	bool isShareable() const {return shareable;}
	bool isShared() const { return refCount > 1;}
private:
	int refCount;
	bool shareable;
};

template<class T> class RCPtr{
public:
	RCPtr(T* p = 0) : pointee(p){ init(); }
	RCPtr(const RCPtr& r) : pointee(r.pointee){ init(); }
	RCPtr& operator=(const RCPtr& r){
		if(pointee != r.pointee)
		{
			if(pointee)
				pointee->removeReference();
			pointee = r.pointee;
			init();
		}
		return *this;
	}
	~RCPtr(){
		if(pointee) pointee->removeReference();
	}
	T* operator->() const { return pointee;}
	T& operator*() const { return *pointee;}
	operator bool () const { if (pointee) return true; return false; }
private:
	void init(){
		if(pointee == 0)
			return;
		if(!pointee->isShareable())
			pointee = new T(*pointee);  // 注意深层拷贝函数
		pointee->addReference();
	}    
	T* pointee; 
	// 注：若StringValue不能派生自RCObject，则定义
	// struct ObjectHolder : public RCObject{
	// ~ObjectHolder() { delete pointee;}
	// T* pointee;
	// };
	// ObjectHolder * holder;
	// 同时要增加 T*operator->() {makecopy(); return holder->pointee;}
	// T&operator*(){makecopy(); return *(holder->pointee);}
	// void makeCopy(){
	// if(holder->isShared()){
	//       T* old = holder->pointee;
	//      holder->removeReference();
	//      holder = new ObjectHolder;
	//      holder->pointee = new T(*old);
	//      holder->addReference();
	// }    
	// }
};
