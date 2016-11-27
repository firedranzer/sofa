#ifndef MYCLASS_H__
#define MYCLASS_H__

class BaseData
{};

template<class T>
class Data : public BaseData
{
private:
    T* t;

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

using namespace std;
class MyClass
{
    public:
        MyClass(){
            initData(&public_datafieldVALID, "name");
        }

        template<class T>
        void initData(BaseData* bd, const std::string& name) ;
        int use(){return 0; }

        Data<int> d_test ;

    private:
};



#endif // MCLASS_H__
