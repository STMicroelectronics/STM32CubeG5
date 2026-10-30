# origin-pack: generated_STMicroelectronics::stfcf@1.1.0
# file-format: 1.0.0
project(generated_STMicroelectronics_stfcf_1_1_0)
cmake_minimum_required(VERSION 3.30)
add_library(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_stfcf_1_1_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STFCF#Cversion:1.1.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STFCF#Csub:MbedTLS Alt Common#Cversion:1.0.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STFCF#Csub:MbedTLS Alt HAL#Cversion:1.0.1#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.stfcf.1.1.0:KWE condition
# description: Requires CCB HAL driver component
set(generated_STMicroelectronics.stfcf.1.1.0_KWE_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:CCB(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_KWE_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:Mbed TLS Alt HAL condition
# description: Mbed TLS Alt HAL condition
set(generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:RNG(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:Mbed TLS Alt STCryptoLib condition
# description: Requires STCryptoLib config component
set(generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_STCryptoLib_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STCryptoLib#.*Csub:CMOX API(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STM32CubeMX2 Config#.*Csub:STCryptoLib(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_STCryptoLib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:PSA interface conditions
# description: Ensure PSA API are satisfied
set(generated_STMicroelectronics.stfcf.1.1.0_PSA_interface_conditions "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:MbedTLS#.*Csub:PSA API(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_PSA_interface_conditions enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:STFCF config condition
# description: STFCF possible implementations
set(generated_STMicroelectronics.stfcf.1.1.0_STFCF_config_condition "$<AND:$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32CubeMX2 Config#.*Csub:RNG(#.*|$)>,>>>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:Core(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_STFCF_config_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes condition
# description: Enable aes_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes hal condition
# description: Enable aes_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_hal_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes stlib condition
# description: Enable aes_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes_aes hal condition
# description: Enable aes_alt_hal implementation based on AES IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_aes_hal_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_aes_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes_ccm hal condition
# description: Enable ccm_alt_hal implementation based on AES IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_ccm_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_ccm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt aes_gcm hal condition
# description: Enable gcm_alt_hal implementation based on AES IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_aes_gcm_hal_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_aes_gcm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ccm condition
# description: Enable ccm_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ccm hal condition
# description: Enable ccm_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ccm stlib condition
# description: Enable ccm_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt chachapoly condition
# description: Enable chachapoly_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_chachapoly_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_chachapoly_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt chachapoly stlib condition
# description: Enable chachapoly_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_chachapoly_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_chachapoly_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt cmac condition
# description: Enable cmac_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_cmac_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_cmac_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt cmac stlib condition
# description: Enable cmac_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_cmac_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_cmac_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt cryp_aes hal condition
# description: Enable aes_alt_hal implementation based on CRYP IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_aes_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_aes_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt cryp_ccm hal condition
# description: Enable ccm_alt_hal implementation based on CRYP IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_ccm_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_ccm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt cryp_gcm hal condition
# description: Enable gcm_alt_hal implementation based on CRYP IP
set(generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_gcm_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_gcm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt drbg condition
# description: Enable drbg_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_drbg_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_drbg_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt drbg stlib condition
# description: Enable drbg_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_drbg_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_drbg_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdh condition
# description: Enable ecdh_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdh hal condition
# description: Enable ecdh_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdh stlib condition
# description: Enable ecdh_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdsa condition
# description: Enable ecdsa_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdsa hal condition
# description: Enable ecdsa_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecdsa stlib condition
# description: Enable ecdsa_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecp condition
# description: Enable ecp_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt STCryptoLib(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt ecp hal condition
# description: Enable ecp_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition "$<OR:$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt entropy_hw condition
# description: Enable entropy_hw_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_entropy_hw_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_entropy_hw_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt gcm condition
# description: Enable gcm_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt gcm hal condition
# description: Enable gcm_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_hal_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt gcm stlib condition
# description: Enable gcm_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt rsa condition
# description: Enable rsa_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt rsa hal condition
# description: Enable rsa_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt rsa stlib condition
# description: Enable rsa_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha1 condition
# description: Enable sha1_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha1 hal condition
# description: Enable sha1_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha256 condition
# description: Enable sha256_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha256 hal condition
# description: Enable sha256_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_hal_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:MbedTLS Alt HAL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha256 stlib condition
# description: Enable sha256_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_stlib_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha512 condition
# description: Enable sha512_alt implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha512 hal condition
# description: Enable sha512_alt_hal implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_hal_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_hal_condition enabled")


# condition: generated_STMicroelectronics.stfcf.1.1.0:alt sha512 stlib condition
# description: Enable sha512_alt_stlib implementation
set(generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_stlib_condition "$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Security#.*Cgroup:STFCF#.*Csub:PSA API(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_stlib_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STM32CubeMX2 Config#Csub:STFCF#Cversion:1.1.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Security_STM32CubeMX2_Config_STFCF_1_1_0")
  target_compile_definitions(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_STFCF_config_condition}:-DCMSIS_USE_generated_Security_STM32CubeMX2_Config_STFCF_1_1_0=1>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_STFCF_config_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STFCF#Csub:MbedTLS Alt Common#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # Router for the Mbed TLS and STMicroelectronics' alternatives according to the configuration choices
  message(DEBUG "Using component generated_Security_STFCF_MbedTLS_Alt_Common_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "-DCMSIS_USE_generated_Security_STFCF_MbedTLS_Alt_Common_1_0_0=1")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common/")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_aes_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_chachapoly_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_cmac_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_drbg_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/common>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Security#Cgroup:STFCF#Csub:MbedTLS Alt HAL#Cversion:1.0.1#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # Mbed TLS alternative implementation thanks hardware acceleration (HAL API)
  message(DEBUG "Using component generated_Security_STFCF_MbedTLS_Alt_HAL_1_0_1")
  target_compile_definitions(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition}:-DCMSIS_USE_generated_Security_STFCF_MbedTLS_Alt_HAL_1_0_1=1>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition}:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/entropy_hardware_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_aes_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_aes_aes_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/aes_aes_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_aes_ccm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/aes_ccm_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_aes_gcm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/aes_gcm_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ccm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_aes_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/cryp_aes_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_ccm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/cryp_ccm_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_cryp_gcm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/cryp_gcm_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ecdh_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/ecdh_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ecdsa_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/ecdsa_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/ecp_alt.c>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_ecp_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/ecp_curves_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_gcm_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_rsa_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/rsa_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha1_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/sha1_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha256_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/sha256_alt.c>")
  target_include_directories(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/include/hal>")
  target_sources(generated_STMicroelectronics_stfcf_1_1_0 INTERFACE "$<$<AND:${generated_STMicroelectronics.stfcf.1.1.0_Mbed_TLS_Alt_HAL_condition},${generated_STMicroelectronics.stfcf.1.1.0_alt_sha512_hal_condition}>:${CMAKE_CURRENT_LIST_DIR}/../../../../../../../../middleware/stfcf/mbedtls_alt/source/hal/sha512_alt.c>")
endif()

