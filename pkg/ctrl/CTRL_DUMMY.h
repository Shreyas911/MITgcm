CBOP
C     !ROUTINE: CTRL_DUMMY.h
C     !INTERFACE:
C     #include "CTRL_DUMMY.h"

C     !DESCRIPTION:
C     *================================================================*
C     | CTRL_DUMMY.h
C     | o Control variables of the ECCO state estimation tool
C     *================================================================*
CEOP

C--   Parameters maxCtrlArr2D, maxCtrlArr3D, maxCtrlTim2D are set in CTRL_SIZE.h
      COMMON /ctrl_dummy_arr/
     &    xx_genarr2d_dummy
     &  , xx_genarr3d_dummy
     &  , xx_gentim2d_dummy
      _RL xx_genarr2d_dummy(maxCtrlArr2D)
      _RL xx_genarr3d_dummy(maxCtrlArr3D)
      _RL xx_gentim2d_dummy(maxCtrlTim2D)

C---+----1----+----2----+----3----+----4----+----5----+----6----+----7-|--+----|
