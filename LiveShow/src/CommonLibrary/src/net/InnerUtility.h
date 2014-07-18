/**
* InnerUtility.h
* 
*
* @version 1.0 (06 ,04, 2008)
* @author reinhardt ken
*
*/
#pragma  once

#include "winsock2.h"
#include <string>
#include <vector>
#include <sstream>
#include "assert.h"
#include "process.h"

#include <algorithm>

namespace InnerUtility
{	
	template<int v> 
	struct Int2Type {
		enum {type = v};
	};

	template <typename T>
	struct ReferenceWrapper {
		explicit ReferenceWrapper(T & Data) 
			: Data_(&Data)	
		{
		}

		operator T& () {
			return *Data_;
		}

	private:
		T * Data_;
	};

	template <typename T> ReferenceWrapper<T> Ref(T& Data) {
		return ReferenceWrapper<T>(Data);
	}

	template <typename T>
	struct RangeVal
	{
		typedef RangeVal this_type;
		typedef T value_type;

		//半开闭区间[From, To)
		RangeVal(const T& From, const T& To)
			: From_(From), To_(To) {
		}

		//特意不用explicit, 便于用于查找
		RangeVal(const T& v)
			: From_(v), To_(v) {
		}

		bool operator < (const this_type& v) const {
			return To_ < v.From_;
		}

		bool operator = (const this_type& v) const {
			return From_ == v.From_ && To_ == v.To_;
		}

		bool operator != (const this_type& v) const {
			return !operator = (v);
		}

		bool IsIn(const T& v) const {
			return v >= From_ && v <= To_;
		}

		T From_;
		T To_;
	};	

	struct Nil
	{
	};

	struct NotNil
	{
	};

	namespace size
	{
		char CalcSize(Nil);
		int CalcSize(...);
	};

	template <typename T>
	struct Size 
	{
		enum {SIZE = sizeof(size::CalcSize(*static_cast<T*>(0)))};
		typedef Int2Type<SIZE> RET;
	};


	//noncopyable.h////////////////////
	class noncopyable {
	protected:
		noncopyable(){}
		virtual ~noncopyable(){}
	private:
		noncopyable(const noncopyable&);
		noncopyable& operator = (const noncopyable&);
	};//end class noncopyable



	//lock.h //////////////////////////////////////////////////////////////////////////////////////////////////

	namespace lock {
		template <typename Lock>
		class scoped_lock : private noncopyable {
		public:
			explicit scoped_lock(Lock& lock) : lock_(lock) {
				lock_.lock();
			}

			~scoped_lock() {
				lock_.unlock();
			}

		private:
			Lock& lock_;
		};//class scoped_lock

		class critical_section/* : private noncopyable*/ {
		public:
			critical_section() {
#if defined(WIN32)
				InitializeCriticalSection(&cs_);
#else
				assert(0);
				//				#error "not support critical_section"
#endif
			}

			~critical_section() {
#if defined(WIN32)
				DeleteCriticalSection(&cs_);
#endif
			}

			void lock() {
#if defined(WIN32)
				EnterCriticalSection(&cs_);
#endif
			}

			void unlock() {
#if defined(WIN32)
				LeaveCriticalSection(&cs_);
#endif
			}			
		private:
#if defined(WIN32)
			CRITICAL_SECTION cs_;
#endif
		};//end class critical_section
	};//namespace lock	

	//thread.h//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct EndGuard
	{
		EndGuard(volatile bool * end)
			: End_(end)
		{
		}

		~EndGuard()
		{
			if (End_) *End_ = true;
		}

	private:
		volatile bool * End_;
	};

	template <typename FUNC>
	class Thread/* : private noncopyable*/ {
		typedef Thread this_type;
	public:
		Thread(FUNC func) : Id_(0), Func_(func), End_(true) {
		}

		~Thread() {
			Stop();
		}

		bool Start(void * param) {
			End_ = false;
			Param_.This = this;
			Param_.Param = param;
#if defined(WIN32)
			unsigned tid;
			Id_ = _beginthreadex(0, 0, ThreadProc, &Param_, 0, &tid);
			if (Id_ == -1) Id_ = 0;
#if _DEBUG
			ThreadId_ = tid;
#endif
#elif defined(_POSIX_THREADS)
			if (pthread_create(&Id_, 0, ThreadProc, &Param_)) {
				Id_ = 0;
			}
#endif
			if (Id_ == 0) {
				End_ = true;
				return false;
			}
			else {
				return true;
			}
		}

		void Stop() {
			if (0 != Id_) {
				while(!End_) {
					Sleep(10);
				}
#if defined(WIN32)
				CloseHandle((HANDLE)Id_);
#endif				
				Id_ = 0;
			}
		}

	private:

		static unsigned __stdcall ThreadProc(void * param)
		{
			THEAD_PARAM * p = static_cast<THEAD_PARAM *>(param);
			this_type * This = static_cast<this_type *>(p->This);
			EndGuard Guard(&This->End_);
			This->Func_(p->Param);
			//This->End_ = true;
			return 0;
		}

	private:
#if defined(WIN32)
		unsigned int Id_;
#if _DEBUG
		unsigned int ThreadId_;
#endif
#elif defined(_POSIX_THREADS)
		pthread_t Id_;
#endif
		struct THEAD_PARAM
		{
			void * This;
			void * Param;
		};
		THEAD_PARAM Param_;
		FUNC Func_;
		volatile bool End_;
	}; //end class Thread;


	//cache.h////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	class Cache //: private noncopyable
	{
	public:
		typedef T data_type;

		inline  explicit Cache(size_t size)
			: Size_(size), Data_(new T[Size_]), Pos_(0), PData_(new T*[Size_]) {
				assert(size);
				for (size_t i = 0; i < size; ++i) {
					PData_[i] = Data_ + i;
				}
		}

		inline  Cache()
			: Size_(0), Data_(0), Pos_(0), PData_(0) {
		}

		inline  explicit Cache(T * array, size_t size)
			: Size_(size), Data_(0), Pos_(0), PData_(new T*[Size_]) {
				assert(size);
				for (size_t i = 0; i < size; ++i) {
					PData_[i] = array + i;
				}
		}	

		inline ~Cache() {
			if(Size_){
				delete []PData_;
				delete []Data_;
			}
		}

		inline T * Get() {
			return PData_[Pos_];
		}

		inline Cache& operator ++() {
			if (++Pos_ == Size_) Pos_ = 0;
			return *this;
		}

		inline Cache& operator --() {
			if (Pos_ == 0) Pos_ = Size_ - 1;
			else --Pos_;
			return *this;
		}

		inline void resize(size_t size){
			assert(Size_ == 0 && size);

			Size_ = size;
			Data_ = new T[Size_];
			PData_ = new T*[Size_];
			
			for (size_t i = 0; i < size; ++i) {
				PData_[i] = Data_ + i;
			}
		}

		inline size_t Size() const {
			return Size_;
		}
	private:
		size_t Size_;
		T * Data_;
		T ** PData_;
		size_t Pos_;
	};

	typedef std::string String;
	typedef std::vector<unsigned char> BUF;
	typedef std::ostringstream OStringStream;
	typedef std::stringstream StringStream;

	typedef lock::critical_section LOCK_OBJ;
	typedef lock::scoped_lock<lock::critical_section> SCOPED_LOCK;


	template <typename OWNER, typename FUNC>
	struct BindMember0
	{
		explicit BindMember0(OWNER& Owner, FUNC Func)
			: Owner_(&Owner), Func_(Func)
		{
		}

		void operator () () {
			(Owner_->*Func_)();
		}

		template <typename ARG>
		void operator () (ARG param) {
			(Owner_->*Func_)(param);
		}

		template <typename ARG1, typename ARG2>
		void operator () (ARG1 param1, ARG2 param2) {
			(Owner_->*Func_)(param1, param2);
		}

		template <typename ARG1, typename ARG2, typename ARG3>
		void operator () (ARG1 param1, ARG2 param2, ARG3 param3) {
			(Owner_->*Func_)(param1, param2, param3);
		}

		template <typename ARG1, typename ARG2, typename ARG3, typename ARG4>
		void operator () (ARG1 param1, ARG2 param2, ARG3 param3, ARG4 param4) {
			(Owner_->*Func_)(param1, param2, param3, param4);
		}		

	private:
		OWNER * Owner_;
		FUNC Func_;
	};

	//shared_ptr.h//////////////////////////////////////////////////////////////////////////////////////////////////
	namespace memory {
		template <bool bMultiThread>
		struct counter_mutex {
		};

		template <>
		struct counter_mutex<true> {
#ifndef WIN32
			mutex mutex_;
#endif
		};
		template <bool bMultiThread>
		class counter {
		public:
			counter(long icount = 0) : _count(icount) {
			}

			counter(const counter& v) : _count(v._count) {
			}

			counter& operator =(const counter& v) {
				if (&v == this) {
					return *this;
				}
				set(_count, v._count, Int2Type<bMultiThread>());
				return *this;
			}

			virtual ~counter() {
			}

			long add_ref() {
				return inc(_count, Int2Type<bMultiThread>());
			}

			long release() {
				if (_count > 0) {
					return dec(_count, Int2Type<bMultiThread>());
				}
				else {
					return 0;
				}
			}

		private:
			inline int inc(volatile long& v, Int2Type<true>) {
#ifdef WIN32
				return InterlockedIncrement((long *)&v);
#else 
				mutex::scoped_lock lock(mutex_.mutex_);
				return ++v;
#endif
			}

			inline int inc(volatile long& v, Int2Type<false>) {
				return ++v;
			}

			inline int dec(volatile long& v, Int2Type<true>) {
#ifdef WIN32
				return InterlockedDecrement((long *)&v);
#else
				mutex::scoped_lock lock(mutex_.mutex_);
				return --v;
#endif
			}

			inline int dec(volatile long& v, Int2Type<false>) {
				return --v;
			}

			inline void set(volatile long& v, long sv, Int2Type<true>) {
#ifdef WIN32
				InterlockedExchange(&v, sv);
#else
				mutex::scoped_lock lock(mutex_.mutex_);
				v = sv;
#endif
			}

			inline void set(volatile long& v, long sv, Int2Type<false>) {
				v = sv;
			}

			volatile long _count;
#ifndef WIN32
			counter_mutex<bMultiThread> mutex_;
#endif

		}; // class counter

		template <typename T, bool bArray, bool bMultiThread>
		class _shared_baseptr {
			class _ptr_data {
				friend class _shared_baseptr<T, bArray, bMultiThread>;
			private:
				explicit _ptr_data() : _p(0), _pcount(0) {
				}

				explicit _ptr_data(T * p, bool barray = false) : _p(p), _pcount(new counter<bMultiThread>(1)) {
				}

				_ptr_data(const _ptr_data& v) : _pcount(v._pcount), _p(v._p) {
				}

				_ptr_data& operator=(const _ptr_data& v) {
					if (this == &v) {
						return *this;
					}
					_pcount = v._pcount;
					_p = v._p;
					return *this;
				}

				long add_ref() {
					if (_pcount) {
						return _pcount->add_ref();
					}
					else return -1;
				}

				long release() {
					if (_pcount) {
						int icount = _pcount->release();
						if (icount == 0) {
							freeptr(_p, Int2Type<bArray>());
							_p = 0;
							delete _pcount;
							_pcount = 0;
						}
						return icount;
					}
					else return -1;
				}

				virtual ~_ptr_data() {
					release();
				}

			private:
				void freeptr(T * p, Int2Type<true>) const {
					delete []p;
				}

				void freeptr(T * p, Int2Type<false>) const {
					delete p;
				}

				T * _p;
				counter<bMultiThread> * _pcount;
			}; //class _ptr_data


			typedef _shared_baseptr<T, bArray, bMultiThread> this_type;
		public:
			_shared_baseptr() : _data() {
			}

			explicit _shared_baseptr(T * p) : _data(p) {
			}

			_shared_baseptr(const _shared_baseptr& v) : _data(v._data) {
				_data.add_ref();
			}

			_shared_baseptr& operator=(const _shared_baseptr& v) {
				if (&v == this) {
					return *this;
				}
				_data.release();
				_data = v._data;
				_data.add_ref();

				return *this;
			}

			virtual ~_shared_baseptr() {
			}

			T& operator [](int index) const {
				return get_element(index, Int2Type<bArray>());
			}

			T& operator * () const {
				return vaddress(Int2Type<bArray>());
			}

			T* operator -> () const {
				return address(Int2Type<bArray>());
			}

			T * get() {
				return _data._p;
			}

			const T * get() const {
				return _data._p;
			}

			bool operator ! () const {
				return _data._p == 0;
			}

			bool bool_type() const {
				return _data._p != 0;
			}

			void swap(this_type & other) {
				std::swap(_data._p, other._data._p);
				std::swap(_data._pcount, other._data._pcount);
			}

			void reset() {
				this_type().swap(*this);
			}

			void reset(T * p) {
				this_type(p).swap(*this);
			}

			bool operator ==(const this_type& v) const {
				return get() == v.get();
			}

			bool operator !=(const this_type& v) const {
				return get() != v.get();
			}

		private:
			T* address(Int2Type<false>) const{
				assert(_data._p);
				return _data._p;
			}

			T& vaddress(Int2Type<false>) const{
				assert(_data._p);
				return *_data._p;
			}

			T& get_element(int index, Int2Type<true>) const {
				assert(_data._p);
				assert(index >= 0);
				return _data._p[index];
			}

			_ptr_data _data;
		}; //class _shared_baseptr

	}; //namespace memory 

	template <typename ARG, typename T = memory::_shared_baseptr<ARG, false, false> >
	struct stshared_ptr : public T {
		stshared_ptr() : T() {
		}

		explicit stshared_ptr(ARG * pArg) : T(pArg){
		}

		stshared_ptr(const stshared_ptr& v) : T(v) {
		}
	};

	template <typename ARG, typename T = memory::_shared_baseptr<ARG, false, true> >
	struct mtshared_ptr : public T {

		mtshared_ptr() : T() {
		}

		explicit mtshared_ptr(ARG * pArg) : T(pArg){
		}

		mtshared_ptr(const mtshared_ptr& v) : T(v) {
		}
	};


	template <typename ARG, typename T = memory::_shared_baseptr<ARG, true, false> >
	struct stshared_array : public T {
		stshared_array() : T() {
		}

		explicit stshared_array(ARG * pArg) : T(pArg){
		}

		stshared_array(const stshared_array& v) : T(v) {
		}
	};

	template <typename ARG, typename T = memory::_shared_baseptr<ARG, true, true> >
	struct mtshared_array : public T {
		mtshared_array() : T() {
		}

		explicit mtshared_array(ARG * pArg) : T(pArg){
		}

		mtshared_array(const mtshared_array& v) : T(v) {
		}
	};

	template <typename ARG, bool bMultiThread, typename T = memory::_shared_baseptr<ARG, bMultiThread, false> >
	struct tshared_ptr : public T {
		tshared_ptr() : T() {
		}

		explicit tshared_ptr(ARG * pArg) : T(pArg){
		}

		tshared_ptr(const tshared_ptr& v) : T(v) {
		}
	};

	template <typename ARG, bool bMultiThread, typename T = memory::_shared_baseptr<ARG, bMultiThread, true> >
	struct tshared_array : public T {
		tshared_array() : T() {
		}

		explicit tshared_array(ARG * pArg) : T(pArg){
		}

		tshared_array(const tshared_array& v) : T(v) {
		}
	};

	//为啥要用define来声明类型呢。。。
	//change by ken 2008 0816
	//shared_ptr和boost冲突，此处用宏，不易修改，故改名
	#ifdef FUND_MULTITHREAD
	#define shared_ptr mtshared_ptr
	#define shared_array mtshared_array
	#else 
	#define shared_ptr2 stshared_ptr
	#define shared_array2 stshared_array
	#endif
};

