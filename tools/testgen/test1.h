#ifndef MYCLASS_H__
#define MYCLASS_H__
class BaseData
{};

template<class T>
class Data : public BaseData
{
private:
    T* m_t;
} ;

template<class T>
class SingleLink
{
private:
    T* m_t;

} ;

template<class T>
class DualLink
{
private:
    T* m_t;

} ;

template<class T>
class OtherType
{
private:
    T* m_t;

} ;



namespace sofa{
class BaseObject
{

};

class MyClass : public BaseObject
{
    public:
        MyClass()
        {
            initData(&d_position, "position");
        }

        void initData(BaseData* b, const std::string& s){}

        Data<int> 			d_position ;
};
}


#endif // MCLASS_H__
