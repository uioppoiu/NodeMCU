#ifndef _AWS_CERT_H_
#define _AWS_CERT_H_

const char *AWS_URL = "a1wcmy64zxk2an-ats.iot.ap-northeast-2.amazonaws.com";
const uint16_t AWS_PORT = 8883;

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVALFRdNN+9KdqF0hzDd9Sd+h4eg86MA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTAyMDUxNDEw
MzZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDQp6JUEPhlmoBU9DMF
Csnpj36XwWxePhkyL7QfkfCMLdkU4+BlymFZLew9KIH4+BmQu0yhNFVZBZrN0lMg
v8PjhZLbnawAKHsJ2pnFIRxq6/MVU01D52wgesYKeWcaP51yuNG74se+rZffQgXU
8q0kc4xU7aCKNX2wx3gKFerTJ1ueBN9MPSDZMKGdku94a5gaDAsnO+EViI7IqtFY
CG9Zbs0CnQFE6nMr6gvMGjE7OcmWTF8/8g83Fls1h+oarrfdRifKVfMzST3HaUv3
dsJUqMMkTZqhKAoQcP61KWKWoXeXvCrwPdSuxJo4TX5OBSSFzIP3YGLYBMEAU+FR
KA2zAgMBAAGjYDBeMB8GA1UdIwQYMBaAFJRWI7CsMa8BNBacpnlwvlr+sEb4MB0G
A1UdDgQWBBSPYivVRa5BXTElrZY2sLG/YpbynTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAFv9JtK7PodtTE5WUYcHx/6w+
VUCVCgfCZI+wkvoY85lP8QYWEvYxwAL0HX3XJEE7RpKPvk4v4rYadp6OvA4XYcbN
I1wuvvieUopbWRIHx5v018KpZt5It0mw3PVyWm/sBUsO08ikuOEQkpv/WvLQ3d0O
VcxvLuHHmWmucufcpeOENMxfv4kRLvxDbcawDMTvs97Byprnqg56px2jYoWvtEqQ
zj9QiDAlwhe+j7NtUpFzZ0KDDbPrRJLLjAbJ3uHs9eXL8DIUc3R7Gg1SLVly2NBZ
Ew7fwpZ+u1UIOzp3lqz2gbfhuib/VCNjmBayj0ED/0TsqsKue6SJeN2nhvN/3Q==
-----END CERTIFICATE-----
)EOF";

// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEA0KeiVBD4ZZqAVPQzBQrJ6Y9+l8FsXj4ZMi+0H5HwjC3ZFOPg
ZcphWS3sPSiB+PgZkLtMoTRVWQWazdJTIL/D44WS252sACh7CdqZxSEcauvzFVNN
Q+dsIHrGCnlnGj+dcrjRu+LHvq2X30IF1PKtJHOMVO2gijV9sMd4ChXq0ydbngTf
TD0g2TChnZLveGuYGgwLJzvhFYiOyKrRWAhvWW7NAp0BROpzK+oLzBoxOznJlkxf
P/IPNxZbNYfqGq633UYnylXzM0k9x2lL93bCVKjDJE2aoSgKEHD+tSlilqF3l7wq
8D3UrsSaOE1+TgUkhcyD92Bi2ATBAFPhUSgNswIDAQABAoIBACwKhhfqcRO5FcdY
uAAjc9TppdXQTNhjkRvnt8RGIJjmAIEJTE8P9ZZGPCv4hd6QN402C4mnKzCcHz28
XDF4aJd4lTPnxutNoBLHYleako36+ZcRuyhPC6KbZ3vEfj+rF8ekPRjEWFbl+z1z
iRmWk3czMHaopY8mlT4gDDVMxQEGVtqkK3DHvMDCdbfrFO58G/jCy/ODfR5/eewq
A25Fog2Pd7vD/5q2yg/2r6Hsu6QHlGu/16oZMw0zGi0+tKk/FtqLd0wGrB3vDgna
3nfn3KIuuHO2YuaRa8Q5krEqvFNwFxPZdOFuCSvYtwiQR6Iyq8n3ruU8vkC51THO
OR/tuZkCgYEA+MDF7e8Sn6VEtcKrabYMlZffufh/JU2MDaaf8iPKQ951YinNzeX5
jvdjScDrGFvDIJ+A1oxX2+CsKPXlC7GJ6UnraVvnmgMXhTxPVPlmL1h0aiI/JZS2
wD8sxLP2koLAWWCJrJD4r073Emk0d4wzpauGNzeVG1lA1ub9MSKYtRcCgYEA1rvN
4qI6mHAQS9KZQuwCiwO9vwy7kwx3v+6dOoJ8rb5rtThYQLNqbXQoAygaUosgHYx4
c2bXDAp63RgI11eV9CHK+FQ5+PGCdxGzYKGsoRYWps0cydPcv82A37Kq6SHYE8wu
1Zhh8w6sLx7tyqhigzCZOtYemtbycPGdsuZGxcUCgYAk6JV1pE9jGKaFQYklejmL
yKkNcu2Kn2L0c5Cv4NeWEl/gXMuIjWLl75bvzowTZSoioQgAzXoLyd5BhZqijYkk
8C7ZxyUqGONYhYZP2WFsDPfY95PfA53KEJWX6gvf2T6pBEXpMOl1OYY3WRzL0CWp
ozWa48hf8nJIcKWIsNVvqQKBgH9kK2CondqRoRlad1IcceXG8mrFQwKaAvD6voxX
nV/1+DAqnTDFP/5vSs2UnQkbQJJHI65S9IFQNlO0Topyi6Op6KAqV7o4jQ3WknRh
deibEhUi2lFiPzQK1snfVUW5rlcQaziMjo+DxOKvlqMEAm2SB1XAcaeqnWoj7UqV
EqeBAoGAK/ZK6XUqgAOZ67ur/jucnU4/4eD/Y9jfDE5i648i3UseXkaWmAk14+kW
wyzS7Ah2dJWaTHTz58GPZHd4mUo7Iu8Y90L/bWZH+EH5NW8vBHNwgefAWwPHf/31
+pOsylCgE9phSJMJltafS4YCwJytY4VPr44ZUz8Jdo/Cftm4Et0=
-----END RSA PRIVATE KEY-----
)EOF";

// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

#endif