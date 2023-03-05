#pragma once

/* --------------------------------------------------------------
 * Copyright (c) 2023 - LUGAND Alexandre
 * --------------------------------------------------------------*/


#define DX3D_V1 1
#define DX3D_V2 0
#define DX3D_V3 0
// numéro de Beta
#define DX3D_V4 1

#define chSTR(x)	   #x
#define chSTR2(x)	chSTR(x)

#define DX3D_VERSION chSTR2(DX3D_V1) "." chSTR2(DX3D_V2) "." chSTR2(DX3D_V3) "." chSTR2(DX3D_V4)
#define DX3D_LEGAL_COPYRIGHT "Copyright © 2023 - LUGAND Alexandre"
