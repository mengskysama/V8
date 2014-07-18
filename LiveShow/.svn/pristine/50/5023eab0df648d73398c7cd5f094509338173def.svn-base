#if !defined(REGULATOR_H)
#define REGULATOR_H

//封装周期调度事件调整功能，每隔周期mUpdatePeriod，isReady返回true；

namespace common 
{
	namespace ui
	{
		template<class TimeType =  long>
		class Regulator
		{

		public:

			Regulator(TimeType updatePeriod)
				:mUpdatePeriod(updatePeriod),mNextUpdateTime(0)
			{ }

			//修改时间间隔
			void SetUpdatePeriod(TimeType updatePeriod) { mUpdatePeriod = updatePeriod; }
			//设置下次更新时间
			void SetNextUpdateTime(TimeType currentTime)
			{
				mNextUpdateTime = currentTime + mUpdatePeriod;
			}
			//当前时间超过下一次执行时间返回真
			bool isReady(TimeType currentTime)
			{

				if( mUpdatePeriod == 0 )   //表示每次都执行
				{
					return true;
				}

				//if( mUpdatePeriod < 0 )   //表示不执行
				if( mUpdatePeriod == static_cast<TimeType>(-1) )   //表示不执行
				{
					return false;
				}


				if( currentTime >= mNextUpdateTime )
				{
					mNextUpdateTime = currentTime + mUpdatePeriod;

					return true;
				}

				return false;
			}

			TimeType GetRestPeriod(TimeType currentTime)
			{
				if (mNextUpdateTime >= currentTime)
				{
					return mNextUpdateTime - currentTime;
				}

				return 0;
			}

			TimeType GetUpdatePeriod()
			{
				return mUpdatePeriod;
			}
		private:

			//执行周期 
			TimeType mUpdatePeriod;

			//下一次执行时间
			TimeType mNextUpdateTime;

		};
	}
}

#endif  //#if !defined(REGULATOR_H)

