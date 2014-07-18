/**
* cyclebuf.h
* this file define the cyclebuf class
*
* @version 1.0 (08 ,24, 2009)
* @author ken
*
*/
#pragma once



template <typename T>
struct CycleBufferNode
{
	CycleBufferNode (bool IsBlank = true)
		: IsBlank_(IsBlank) {
	}

	volatile bool IsBlank_;
	T Data_;
};



template <typename DATA>
class CycleBuffer 
{
	//缺省拷贝构造可以很好的工作
public:
	typedef DATA data_type;

	CycleBuffer(int MaxCount = 1024)
		: MaxCount_(MaxCount), Nodes_(MaxCount), WriteIndex_(0), ReadIndex_(0) {
			assert(MaxCount);
	}

	~CycleBuffer() {
	}

	bool Write(const DATA& data) {
		bool bok;
		if (Nodes_[WriteIndex_].IsBlank_) {
			Nodes_[WriteIndex_].Data_ = data;
			Nodes_[WriteIndex_].IsBlank_ = false;
			bok = true;
		}
		else bok = false;
		if (bok) {
			if (WriteIndex_ < MaxCount_-1) ++WriteIndex_;
			else WriteIndex_ = 0;
		}
		return bok;
	}

	bool Read(DATA& data) {
		bool bok;
		if (!Nodes_[ReadIndex_].IsBlank_) {
			data = Nodes_[ReadIndex_].Data_;
			Nodes_[ReadIndex_].IsBlank_ = true;
			bok = true;
		}
		else bok = false;
		if (bok) {
			if (ReadIndex_ < MaxCount_-1) ++ReadIndex_;
			else ReadIndex_ = 0;
		}
		return bok;
	}

	bool ReadWithReset(DATA& data) {
		bool bok;
		if (!Nodes_[ReadIndex_].IsBlank_) {
			data = Nodes_[ReadIndex_].Data_;
			Nodes_[ReadIndex_].Data_ = DATA();
			Nodes_[ReadIndex_].IsBlank_ = true;
			bok = true;
		}
		else bok = false;
		if (bok) {
			if (ReadIndex_ < MaxCount_-1) ++ReadIndex_;
			else ReadIndex_ = 0;
		}
		return bok;
	}

	int Size() const {
		return MaxCount_;
	}
private:
	typedef std::vector<CycleBufferNode<DATA> > NODES;
	NODES Nodes_;
	const int MaxCount_;
	int WriteIndex_;
	int ReadIndex_;
}; //class CycleBuffer




