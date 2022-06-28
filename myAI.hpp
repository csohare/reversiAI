#include "OthelloAI.hpp"
#include <vector>

namespace csohare
{
    class ohareAI: public OthelloAI
    {
        public: 
            std::pair<int, int> chooseMove(const OthelloGameState& state) override;
            std::vector<std::pair<int, int>> options(const OthelloGameState& state);
            int evalState(const OthelloGameState& state);
            int search(std::unique_ptr<OthelloGameState>& state, int depth, int player);

    };
}
