#include "Smoother.h"

namespace comp_prop_kit_lib {
    namespace smoother {
        SmootherValue SmootherLinear::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {
            {

                // y^
                //  |
                //  |
                // 1-          O
                //  |        OO
                //  |      OO
                //  |     O 
                //  |   OO
                //  | OO  
                //  |O 
                // -+----------|-->
                // 0|          1  x


                return value * (max - min) + min;
            }
        }

        SmootherValue SmootherSmoothLinear::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {

            // ((6x - 15)x + 10)x^3
            // (6x^2 - 15x + 10)x^3
            // 6x^5 - 15x^4 + 10x^3

            // y^
            //  |
            //  |
            // 1-         OO
            //  |       OO
            //  |      O
            //  |     O
            //  |    O
            //  |  OO
            //  |OO
            // -+----------|-->
            // 0|          1  x

            // Why not (-cos(pi * x) + 1) / 2?
            // It does basically same as the equation

            // Why not? it's because a speed

            return (((6 * value - 15) * value + 10) * value * value * value) * (max - min) + min;
        }

        SmootherValue SmootherStep::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {

            // y^
            //  |
            //  |
            // 1-     OOOOOO
            //  |          
            //  |        
            //  |       
            //  |     
            //  |     
            //  |OOOOO
            // -+-----|----|-->
            // 0|    0.5   1  x

            return value < 0.5 ? min : max;
        }

        SmootherValue SmootherTwoSteps::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {

            // y^
            //  |
            //  |
            // 1-       OOOO
            //  |          
            //  |        
            // .|       
            // 5-   OOOO  
            //  |     
            //  |OOO  
            // -+--|----|--|-->
            // 0| .25  .75 1  x

            if (value < 0.25)
                return min;
            else if (value > 0.75)
                return max;
            else
                return (min + max) / 2.0;
        }

        SmootherValue SmootherParabola::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {

            // y^
            //  |
            //  |
            // 1-          O
            //  |         O
            //  |         O
            //  |        O
            //  |      OO 
            //  |   OOO 
            //  |OOO  
            // -+----------|-->
            // 0|          1  x

            return value * value * (max - min) + min;
        }
        SmootherValue SmootherInverseParabola::fitValue(SmootherValue value, SmootherValue min, SmootherValue max) {

            // y^
            //  |
            //  |
            // 1-       OOOO
            //  |     OO   
            //  |   OO     
            //  |  O      
            //  | O       
            //  |O      
            //  |O    
            // -+----------|-->
            // 0|          1  x

            return sqrt(value) * (max - min) + min;
        }
    }
}
