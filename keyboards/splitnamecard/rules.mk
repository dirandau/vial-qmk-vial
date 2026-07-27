BOARD = GENERIC_RP_RP2040

# 다이렉트 핀 스캐너 드라이버 명시적 활성화 (필수!)
MATRIX_DRIVER = direct

# 포인팅 디바이스(마우스) 기능 및 PMW3360 드라이버 활성화
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pmw3360

# SPI 통신 활성화 (RP2040 하드웨어 SPI 사용)
SPI_ENABLE = yes
SPI_DRIVER = SPI