#include <vector>
namespace IODevices
{
    typedef struct Coordinates {
        int x;
        int y;
    } Position;

    struct Move{
   		Position origin;
    	Position dest;
    }

    class HalController{
        private:
            std::vector<int>
            Move origin;
            Move dest;
        public:
            HalController();
            virtual ~HalController();
            
            void scan();
    };
}