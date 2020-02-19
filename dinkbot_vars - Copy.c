#include <stdint.h>

const int16_t v_L_forward = 220,
        v_R_forward = 220,
        v_L_backUp = -220,
        v_R_backUp = -220,
        v_L_turn90cw = 300,
        v_R_turn90cw = -300,
        v_L_turn90ccw = -300,
        v_R_turn90ccw = 300,
        v_L_turn10LBack = -300,
        v_R_turn10LBack = 0,
        v_L_turn10RBack = 0,
        v_R_turn10RBack = -300,
        v_L_turn30LBack = -250,
        v_R_turn30LBack = 0,
        v_L_turn30RBack = 0,
        v_R_turn30RBack = -250,
        v_L_turn30LCenter = -250,
        v_R_turn30LCenter = -200,
        v_L_turn30RCenter = -200,
        v_R_turn30RCenter = -250,
        v_eject = 500,
        v_lift = 500,
        v_retract = -400,
        v_L_fixPos = 400,
        v_R_fixPos = 400;

const uint16_t t_turn90 = 370,
        t_backUp = 400,
        t_forward1 = 2000,
        t_turn10Back = 90,
        t_turn30Back = 300,
        t_evade = 800,
        t_backUpFix = 200,
        t_smallBackup = 30,
        t_eject = 1200,
        t_lift1 = 400,
        t_lift2 = 2350,
        t_retract = 2000,
        t_fixPos = 2000,
        t_shake = 50,
        t_forward = 2000,
        t_forward2 = 2400,
        t_turn30 = 400,
        t_turn30Center = 800;