
#include <cartobase/smart/rcptr.h>


namespace carto
{

    /** This test class just counts the number of instances of this type,
        in order to allow rc_ptr debugging.
    */
    class TestRCObject : public RCObject
    {
    public:
        TestRCObject( bool verbose = false );
        virtual ~TestRCObject();
        static unsigned long globalCount();
        static RCObject* newTestObject();
        void setVerbose( bool verbose );
        bool verbose() const { return _verbose; }

    private:
        bool _verbose;
        static unsigned long global_count;
    };

}

