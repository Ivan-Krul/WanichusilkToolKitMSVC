#include <iostream>

#include "NewFloatVar/FloatVar.h"
#include "NewFloatVar/FloatVarNumeric.h"
#include "NewFloatVar/FloatVarString.h"
#include <raylib.h>

#include <chrono>

struct TimeStop {
    std::chrono::high_resolution_clock::time_point beg, end;

    TimeStop() {
        beg = end = std::chrono::high_resolution_clock::now();
    }

    float stop() {
        end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count() / 1000.0f;
    }
};

int main(int argc, char* argv[]) {
    printf("Local path: %s\n", argv[0]);

    hardware_envi_lib::FloatVarNumeric fvnf = 0;
    {
        hardware_envi_lib::FloatVar fv;
        fv.name = "What";
        memcpy(fv.data_type, "flot", 5);
        fv.setBinaryValue(5.3f);

        fvnf = fv;
    }
    hardware_envi_lib::FloatVarNumeric fvn = 65;
    hardware_envi_lib::FloatVarString fvs = "whar";
    fvs()->name = "no";

    std::cout << "FloatVar: " << fvn()->data_type << " " << fvn()->name << " = " << fvn.getValue<int>() << '\n';
    std::cout << "FloatVar: " << fvnf()->data_type << " " << fvnf()->name << " = " << fvnf.getValue<float>() << '\n';
    std::cout << "FloatVar: " << fvs()->data_type << " " << fvs()->name << " = " << fvs.getValue() << '\n';

    return 0;
}

