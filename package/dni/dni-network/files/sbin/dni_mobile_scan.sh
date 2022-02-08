#!/bin/sh

CONFIG=/bin/config

# Mobile Network Related Database
# Principle
# ICCID_iccid_num=Country*ISP*APN*PdpType*AuthType*Username*Password


ICCID_8943170_1="AUS*spusu*mass.at*IPV4V6*none***"

ICCID_9024902_1="SUD*MTN*internet1*IPV4*none***"
ICCID_8999893_1="UZB*Ucell*internet*IPV4*none***"
ICCID_8999807_1="UZB*MTS*net.mts.uz*IPV4*PAP*mts*mts*"
ICCID_8999650_1="KGZ*BITEL*Beeline.internet.kg*IPV4*none***"
ICCID_8999597_1="GEO*Magtigsm*3g.ge*IPV4*none***"
ICCID_8999504_1="GEO*BEELINE*internet.ge.beeline.net*IPV4*none***"
ICCID_8999501_1="GEO*Geocell*internet*IPV4*none***"
ICCID_8999455_1="AZE*BKCELL*gprs*IPV4*none***"
ICCID_8999450_1="AZE*VODA*internet*IPV4*none***"
ICCID_8999404_1="AZE*Nar Mobile*azerfon*IPV4*PAP*azerfon*azerfon*"
ICCID_8999401_1="AZE*ACELL*internet*IPV4*none***"
ICCID_8999302_1="TKM*Altyn Asyr*gprs.tmcell*IPV4*none***"
ICCID_8999203_1="TJK*MLT*internet*IPV4*none***"
ICCID_899843_1="IRI*Irancell*mtnirancell*IPV4*none***"
ICCID_899820_1="IRI*RighTel*RighTel*IPV4*none***"
ICCID_899811_1="IRI*MCI*mcinet*IPV4*none***"
ICCID_8997702_1="NEP*Ncell*web*IPV4*none***"
ICCID_8997511_1="BHU*B-Mobile*internet*IPV4*none***"
ICCID_8997401_1="QAT*Ooredoo*data*IPV4*none***"
ICCID_8997304_1="BHR*VIVA*viva.bh*IPV4*none***"
ICCID_8997302_1="BHR*Zain BH*internet*IPV4*PAP*internet*internet*"
ICCID_8997301_1="BHR*BATELCO*internet.batelco.com*IPV4*none***"
ICCID_8997250_1="ISR*Pelephone*internet.pelephone.net.il *IPV4*PAP*pcl@3g*pcl*"
ICCID_8997207_1="ISR*Hot Mobile*net.hotm*IPV4*none***"
ICCID_8997202_1="ISR*Cellcom Israel*internetg*IPV4*PAP*guest*guest*"
ICCID_8997201_1="ISR*Orange IL*uinternet*IPV4*PAP*orange*orange*"
ICCID_8997112_1="ARE*ETISALAT*etisalat.ae*IPV4*none***"
ICCID_8997103_1="ARE*du*du*IPV4*none***"
ICCID_8996803_1="OMN*nawras*isp.nawras.com.om*IPV4*PAP*test*test*"
ICCID_8996802_1="OMN*OMAN MOBILE*taif*IPV4*PAP*taif*taif*"
ICCID_8996641_1="SAU*Mobily*web2*IPV4*none***"
ICCID_899663_1="SAU*Mobily*web2*IPV4*none***"
ICCID_8996624_1="SAU*Mobily*web2*IPV4*none***"
ICCID_8996613_1="SAU*Mobily*web2*IPV4*none***"
ICCID_8996606_1="SAU*Lebara*lebara*IPV4*none***"
ICCID_8996604_1="SAU*Zain SAU*zain*IPV4*none***"
ICCID_8996603_1="SAU*Mobily*web2*IPV4*none***"
ICCID_8996601_1="SAU*STC*jawalnet.com.sa*IPV4*none***"
ICCID_899659_1="KWT*zain KW*pps*IPV4*PAP*pps*pps*"
ICCID_8996504_1="KWT*VIVA KUWAIT*viva*IPV4*none***"
ICCID_8996503_1="KWT*Ooredoo*action.ooredoo.com*IPV4*none***"
ICCID_8996405_1="IRQ*ASIACELL*net.asiacell.com*IPV4*none***"
ICCID_8996277_1="JOR*Orange*net.orange.jo*IPV4*PAP*net*net*"
ICCID_8996203_1="JOR*UMINIAH*net*IPV4*none***"
ICCID_8996201_1="JOR*zain*internet*IPV4*none***"
ICCID_8996103_1="LIB*MTC LIBA*gbrs.mtctouch.com.lb*IPV4*none***"
ICCID_8996101_1="LIB*Alfa*internet.mic1.com.lb*IPV4*PAP*mic1*mic1*"
ICCID_8996101_2="LIB*Alfa*usb.mic1.com.lb*IPV4*none***"
ICCID_8996002_1="MDV*WATANIYA*WataniyaNet*IPV4*none***"
ICCID_8996001_1="MDV*Dhiraagu*internet.dhimobile*IPV4*none***"
ICCID_899506_1="MYA*TM Touch*internet*IPV4*none***"
ICCID_899505_1="MYA*Ooredoo*internet*IPV4*none***"
ICCID_899501_1="MYA*MPTGSM*mptnet*IPV4*none***"
ICCID_899470_1="SRI*VODA*internet*IPV4*none***"
ICCID_899408_1="SRI*Hutch*data*IPV4*none***"
ICCID_899405_1="SRI*airtel*data*IPV4*none***"
ICCID_899402_1="SRI*DIALOG*dialogbb*IPV4*none***"
ICCID_899401_1="SRI*MOBITEL*isp*IPV4*none***"
ICCID_899350_1="AFG*Etisalat*etisalat.af.web*IPV4*none***"
ICCID_899320_1="AFG*ROSHAN*internet*IPV4*none***"
ICCID_899301_1="AFG*AWCC*internet*IPV4*PAP*awcc*1111*"
ICCID_899230_1="PAK*Mobilink*connect.mobilinkworld.com*IPV4*none***"
ICCID_899204_1="PAK*ZONG*zonginternet*IPV4*none***"
ICCID_899197_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899194_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899190_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899187_1="IND*LoopMobi*www*IPV4*PAP*loop*loop*"
ICCID_899186_1="IND*Limited Service*airtelgprs.com*IPV4*none***"
ICCID_899186_2="IND*JIO 4G *jionet*IPV4*none***"
ICCID_899186_3="IND*LoopMobi*www*IPV4*PAP*loop*loop*"
ICCID_899186_4="IND*JIO 4G*jionet*IPV4*none***"
ICCID_899185_1="IND*JIO 4G*jionet*IPV4*none***"
ICCID_899180_1="IND*Aircel*aircelgprs*IPV4*none***"
ICCID_899173_1="IND*CellOne*bsnlnet*IPV4*none***"
ICCID_899172_1="IND*BSNL Mobile*bsnlnet*IPV4*none***"
ICCID_899166_1="IND*Vodafone*www*IPV4*none***"
ICCID_899149_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899146_1="IND*Vodafone*www*IPV4*none***"
ICCID_899145_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899143_1="IND*Vodafone*www*IPV4*none***"
ICCID_899140_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899136_1="IND*RELIANCE*rcomnet*IPV4*none***"
ICCID_899122_1="IND*IDEA*internet*IPV4*none***"
ICCID_899120_1="IND*Vodafone*www*IPV4*none***"
ICCID_899111_1="IND*IDEA*internet*IPV4*none***"
ICCID_899110_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899107_1="IND*Oasis*data*IPV4*none***"
ICCID_899105_1="IND*Vodafone*www*IPV4*none***"
ICCID_899104_1="IND*IDEA*internet*IPV4*none***"
ICCID_899103_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899100_1="IND*AIRTEL*airtelgprs.com*IPV4*none***"
ICCID_899028_1="TUR*AVEA*Internet*IPV4*none***"
ICCID_899002_1="TUR*VODAFONE TR*vodafone*IPV4*none***"
ICCID_899001_1="TUR*TR TURKCELL*internet*IPV4*none***"
ICCID_8988697_1="TPE*Taiwan Mobile*internet*IPV4*none***"
ICCID_8988693_1="TPE*Taiwan Mobile*internet*IPV4*none***"
ICCID_8988692_1="TPE*Chunghwa*internet*IPV4*none***"
ICCID_8988689_1="TPE*Taiwan Star*internet*IPV4*none***"
ICCID_8988605_1="TPE*GT 4G*gtnet*IPV4*none***"
ICCID_8988601_1="TPE*Far EasTone*internet*IPV4*none***"
ICCID_8988038_1="BAN*Banglalink*blweb*IPV4*none***"
ICCID_8988008_1="BAN*TeleTalk*data*IPV4*none***"
ICCID_898611_1="CHN*CTCC*ctnet*IPV4*PAP*ctnet@mycdma.cn*vnet.mobi*"
ICCID_898606_1="CHN*China Unicorn*3gnet*IPV4*none***"
ICCID_898603_1="CHN*CTCC*ctnet*IPV4*PAP*ctnet@mycdma.cn*vnet.mobi*"
ICCID_898601_1="CHN*CU-GSM*3gnet*IPV4*none***"
ICCID_898600_1="CHN*CMCC*cmnet*IPV4*none***"
ICCID_8985601_1="LAO*Telcom*ltcnet*IPV4*none***"
ICCID_8985508_1="CAM*Metfone*metfone*IPV4*none***"
ICCID_8985506_1="CAM*Smart Mobile*smart*IPV4*none***"
ICCID_8985501_1="CAM*Cellcard*Cellcard*IPV4*PAP*mobitel*mobitel*"
ICCID_8985303_1="MAC*3 MO*3mobile.three.com.mo*IPV4*none***"
ICCID_8985301_1="MAC*CTM*mobile.three.com.mo*IPV4*none***"
ICCID_8985300_1="MAC*SmarTone*smartgprs*IPV4*none***"
ICCID_8985221_1="HKG*21 Vianet*21VN*IPV4*none**Add this to the manual*"
ICCID_8985212_1="HKG*China Mobile HK*cmhk*IPV4*none***"
ICCID_8985207_1="HKG*UNICOM HK*3gnet*IPV4*none***"
ICCID_8985206_1="HKG*SmarTone HK*smartone*IPV4*none***"
ICCID_8985206_2="HKG*Smartone HK Real IP*internet*IPV4*none***"
ICCID_8985200_1="HKG*csl.*mobile*IPV4*none***"
ICCID_8985200_2="HKG*SUN Mobile*SUNMobile*IPV4*none***"
ICCID_898445_1="VIE*VODO*yesinternet*IPV4*none***"
ICCID_898405_1="VIE*Vietnamo*vietnamobile*IPV4*none***"
ICCID_898404_1="VIE*VIETTEL*v-internet*IPV4*none***"
ICCID_898402_1="VIE*Vinaphone*m3-world*IPV4*PAP*mms*mms*"
ICCID_898401_1="VIE*mobifone*m-wap*IPV4*PAP*mms*mms*"
ICCID_898230_1="KOR*KT*lte.ktfwing.com*IPV4*none***"
ICCID_898206_1="KOR*LG U+*internet.lguplus.co.kr*IPV4*none***"
ICCID_898205_1="KOR*SKTelecom*lte.sktelecom.com*IPV4*none***"
ICCID_898110_1="JPN*moperaU Foma*mopera.net *IPV4*none***"
ICCID_898110_2="JPN*Rakuten Mobile*rmobile.co*IPV4*PAP_CHAP*rm*0000*"
ICCID_898110_3="JPN*mineo D mobile*mineo-d.jp*IPV4*CHAP*mineo@k-opti.com*mineo*"
ICCID_898110_4="JPN*OCN Mobile ONE*lte-d.ocn.ne.jp*IPV4*PAP_CHAP*mobileid@ocn*mobile*"
ICCID_898110_5="JPN*IIJmio Mobile*iijmio.jp*IPV4*PAP_CHAP*mio@iij*iij*"
ICCID_898110_6="JPN*BIGLOBE MOBILE*biglobe.jp*IPV4*PAP_CHAP*user*0000*"
ICCID_898110_7="JPN*AEON Mobile*i-aeonmobile.com*IPV4*PAP_CHAP*user*0000*"
ICCID_898110_8="JPN*LINE Mobile*line.me*IPV4*PAP_CHAP*line@line*line*"
ICCID_898110_9="JPN*DMM Mobile*dmm.com*IPV4*PAP_CHAP*dmm*dmm*"
ICCID_898110_10="JPN*sp mode*spmode.ne.jp*IPV4*none***"
ICCID_898110_11="JPN*excite Mobile*vmobile.jp*IPV4*PAP_CHAP*bb@excite.co.jp*excite*"
ICCID_898110_12="JPN*JP Mobile*jpsim.me*IPV4*PAP_CHAP*jps@jps*888*"
ICCID_898110_13="JPN*nuro Mobile*so-net.jp*IPV4*PAP_CHAP*nuro*nuro*"
ICCID_898110_14="JPN*OCN Mobile ONE*lte.ocn.ne.jp*IPV4*PAP_CHAP*mobileid@ocn*mobile*"
ICCID_898103_1="JPN*IIJmio Mobile*iijmio.jp*IPV4*PAP_CHAP*mio@iij*iij*"
ICCID_8971100_1="RUS*250 - 11*internet.yota.ru*IPV4*none***"
ICCID_8970199_1="RUS*Beeline*internet.beeline.ru*IPV4*PAP*beeline*beeline*"
ICCID_8970139_1="RUS*ROSTELECOM*internet.rt.ru*IPV4*none***"
ICCID_8970135_1="RUS*MOTIV*inet.ycc.ru*IPV4*PAP*motiv*motiv*"
ICCID_8970120_1="RUS*TELE2*internet.tele2.ru*IPV4*none***"
ICCID_8970116_1="RUS*New Telephone*internet.ntc*IPV4*none***"
ICCID_8970115_1="RUS*SMARTS-Ufa*internet.smarts.ru*IPV4*none***"
ICCID_8970113_1="RUS*MTS*internet.mts.ru*IPV4*PAP*mts*mts*"
ICCID_8970112_1="RUS*Baykalwestcom*internet.rt.ru*IPV4*none***"
ICCID_8970111_1="RUS*250 - 11*internet.yota.ru*IPV4*none***"
ICCID_8970107_1="RUS*SMARTS*internet.smarts.ru*IPV4*none***"
ICCID_8970105_1="RUS*Yeniseytelecom*internet.rt.ru*IPV4*none***"
ICCID_8970103_1="RUS*Rostelecom OJSC*internet.rt.ru*IPV4*none***"
ICCID_8970102_1="RUS*MegaFon*internet*IPV4*none***"
ICCID_8970101_1="RUS*MTS*internet.mts.ru*IPV4*PAP*mts*mts*"
ICCID_8968920_1="PYF*VINI*internet*IPV4*none***"
ICCID_8968915_1="PYF*vodafone*internet*IPV4*none***"
ICCID_8968701_1="NCD*Mobilis*Im*IPV4*none***"
ICCID_8968700_1="NCD*VODA*internet*IPV4*none***"
ICCID_8968527_1="SAM*GO*4g.bluesky*IPV4*none***"
ICCID_8968500_1="SAM*DIGICEL*internet*IPV4*none***"
ICCID_8968201_1="COK*KOKANET*kotaa*IPV4*none***"
ICCID_8967902_1="FIJ*Digicel*web.digicelpacific.com*IPV4*none***"
ICCID_8967901_1="FIJ*Vodafone*live.vodafone.com.fj*IPV4*none***"
ICCID_8967805_1="VAN*Digicel*web.digicelpacific.com*IPV4*none***"
ICCID_8967801_1="VAN*SMILE*tvlweb.net.vu*IPV4*none***"
ICCID_8967688_1="TGA*Digicel*Web.digicelpacific.com*IPV4*none***"
ICCID_8967617_1="TGA*OceanCell*oceancell*IPV4*none***"
ICCID_8967503_1="PNG*DIGICEL*internet.digicelpng.com*IPV4*none***"
ICCID_8967381_1="BRU*B-Mobile*bmobilewap*IPV4*none***"
ICCID_8967072_1="TLS*Timor Telecom*internet*IPV4*none***"
ICCID_896615_1="THA*ACT-1900*internet*IPV4*none***"
ICCID_896605_1="THA*DTAC*www.dtac.co.th*IPV4*none***"
ICCID_896604_1="THA*TrueMove H*hinternet*IPV4*PAP*true*true*"
ICCID_896603_1="THA*AIS*internet*IPV4*none***"
ICCID_896601_1="THA*AIS*internet*IPV4*none***"
ICCID_896600_1="THA*TH 3G+*internet*IPV4*none***"
ICCID_8965501_1="ZAF*VodaCom*lte.vodacom.za*IPV4*none***"
ICCID_896510_1="SGP*TPG Telcom*tpg*IPV4*none***"
ICCID_896505_1="SGP*StarHub*shwap*IPV4*none***"
ICCID_896503_1="SGP*M1*sunsurf*IPV4*PAP*65*65*"
ICCID_896501_1="SGP*SingTel*e-ideas*IPV4*PAP*65ideas*65ideas*"
ICCID_896450_1="NZL*MTN ZM*internet*IPV4*none***"
ICCID_896424_1="NZL*2degrees*internet*IPV4*none***"
ICCID_896410_1="NZL*Vodafone UG*internet*IPV4*none***"
ICCID_896405_1="NZL*Spark*internet*IPV4*none***"
ICCID_896401_1="NZL*Vodafone NZ*vodafone*IPV4*none***"
ICCID_896340_1="PHI*Sudani*Subaninet*IPV4*none***"
ICCID_896303_1="PHI*SMART*internet*IPV4*none***"
ICCID_896289_1="INA*3 INA*3data*IPV4*PAP*3data*3data*"
ICCID_896228_1="INA*Smartfren*smartfren4g*IPV4*PAP*Smartfren*Smartfren*"
ICCID_896211_1="INA*XL*www.xlgprs.net*IPV4*PAP*xlgprs*proxl*"
ICCID_896210_1="INA*Telkomsel*internet*IPV4*none***"
ICCID_896209_1="INA*Smartfren*smartfren4g*IPV4*PAP*Smartfren*Smartfren*"
ICCID_896201_1="INA*INDOSAT*Indosatgprs*IPV4*PAP*indosatgprs*indosatgprs*"
ICCID_896101_1="AUS*ALDImobile*mdata.net.au*IPV4V6*none***"
ICCID_896101_2="AUS*Telstra*telstra.internet*IPV4V6*none***"
ICCID_896080_1="MAL*VODA*internet*IPV4*none***"
ICCID_896019_1="MAL*CELCOM*celcom3g*IPV4*none***"
ICCID_896018_1="MAL*U Mobile*my3g*IPV4*none***"
ICCID_896016_1="MAL*DiGi*3dgdnet*IPV4*none***"
ICCID_896015_1="MAL*Yes 4G*yesnet*IPV4*none***"
ICCID_896001_1="MAL*MY MAXIS*unet*IPV4*none***"
ICCID_8959991_1="BQN*UTS*utsnet.uts.an*IPV4*none***"
ICCID_8959969_1="BQN*Digicel*web.digicelcuracao.com*IPV4*none***"
ICCID_8959801_1="URU*ANTEL*antel.lte*IPV4*none***"
ICCID_8959703_1="SUR*Digicel*web.digicelsr.com*IPV4*none***"
ICCID_8959702_1="SUR*TeleG*win.teleg.sr*IPV4*none***"
ICCID_8959620_1="MTQ*DigicelF*web.digicelfr.com*IPV4*none***"
ICCID_8959602_1="MTQ*ONLY*internet.sfr*IPV4*none***"
ICCID_8959504_1="PAR*Tigo*internet.tigo.py*IPV4*none***"
ICCID_8959501_1="PAR*VOX*vox.internet*IPV4*PAP*vox*vox*"
ICCID_8959402_1="GUN*ONLY*sfr*IPV4*none***"
ICCID_8959400_1="GUN*Voda*internet*IPV4*none***"
ICCID_8959301_1="ECU*CLARO*internet.claro.com.ec*IPV4*none***"
ICCID_8959300_1="ECU*movistar*internet.movistar.com.ec*IPV4*PAP*movistar*movistar*"
ICCID_8959100_1="BOL*Claro*internet*IPV4*none***"
ICCID_8959002_1="GDP*ONLY*internet.sfr*IPV4*none***"
ICCID_8959001_1="GDP*Orange*orangeweb*IPV4*none***"
ICCID_895804_1="VEN*movistar*internet.movistar.ve*IPV4*none***"
ICCID_895802_1="VEN*DIGITEL*gprsweb.digitel.ve*IPV4*none***"
ICCID_895773_1="COL*Columbia Mobile*web.colombiamovil.com.co*IPV4*PAP*web-cm1900*web-cm1900*"
ICCID_895747_1="COL*VODA*internet*IPV4*none***"
ICCID_895714_1="COL*UNE*www.une.net.co*IPV4*PAP*une*une*"
ICCID_895712_1="COL*movistar*internet.movistar.com.co*IPV4*PAP*movistar*movistar*"
ICCID_895710_1="COL*COMCEL*internet.comcel.com.co*IPV4*PAP*comcelweb*comcelweb*"
ICCID_895603_1="CHI*ClaroCHL*Bam.clarochile.cl*IPV4*PAP*clarochile*clarochile*"
ICCID_895602_1="CHI*Movistar*wap.tmovil.cl*IPV4*PAP*wap*wap*"
ICCID_895601_1="CHI*ENTELPCS*bam.entelpcs.cl*IPV4*PAP*entelpcs*entelpcs*"
ICCID_895531_1="BRA*Oi*gprs.oi.com.br*IPV4*PAP*oi*oi*"
ICCID_895511_1="BRA*VIVO*zap.vivo.com.br*IPV4*PAP*vivo*vivo*"
ICCID_895510_1="BRA*VIVO*zap.vivo.com.br*IPV4*PAP*vivo*vivo*"
ICCID_895505_1="BRA*Claro*claro.com.br*IPV4*PAP*claro*claro*"
ICCID_895504_1="BRA*TIM*timbrasil.br*IPV4*PAP*tim*tim*"
ICCID_895503_1="BRA*TIM*timbrasil.br*IPV4*PAP*tim*tim*"
ICCID_895502_1="BRA*TIM*tim.br*IPV4*PAP*tim*tim*"
ICCID_895500_1="BRA*VODA*internet*IPV4*none***"
ICCID_895470_1="ARG*ViTi*internet*IPV4*none***"
ICCID_895434_1="ARG*AR TP*internet.personal.com*IPV4*none***"
ICCID_895407_1="ARG*Movistar*wap.gprs.unifon.com.ar*IPV4V6*PAP*wap*wap*"
ICCID_895431_1="ARG*Claro AR*igprs.claro.com.ar*IPV4*none***"
ICCID_895380_1="CUB*VODA*data*IPV4*none***"
ICCID_895205_1="MEX*iUSACELL*web.iusacellgsm.mx*IPV4*PAP*iusacellgsm*iusacellgsm*"
ICCID_895202_1="MEX*TELCEL*Internet.itelcel.com*IPV4*PAP*webgprs*webgprs2002*"
ICCID_895200_1="MEX*VODA*internet*IPV4*none***"
ICCID_895147_1="PER*VODA*data*IPV4*none***"
ICCID_895115_1="PER*Bitel*bitel.pe*IPV4*none***"
ICCID_895110_1="PER*Claro*claro.pe*IPV4*PAP*claro*claro*"
ICCID_895106_1="PER*MOVISTAR*movistar.pe*IPV4*PAP*movistar@datos*movistar*"
ICCID_8950704_1="PAN*Digicel*web.digicelpanama.com*IPV4*none***"
ICCID_8950703_1="PAN*CLARO*web.claro.com.pa*IPV4*PAP*claro*claro*"
ICCID_8950701_1="PAN*Cable & Wireless*apn01.cwpanama.com.pa*IPV4*none***"
ICCID_8950603_1="CRC*CLARO*internet.ideasclaro*IPV4*none***"
ICCID_8950601_1="CRC*I.C.E.*kolbi3g*IPV4*none***"
ICCID_8950604_1="CRC*Movistar*internet.movistar.cr*IPV4*PAP*wap*wap*"
ICCID_8950400_1="HON*ClaroHND*internet.ideasclaro*IPV4*none***"
ICCID_8950202_1="GUA*TIGO*broadband.tigo.gt*IPV4*none***"
ICCID_8950167_1="BIZ*Digicell*www.btlgprs.bz*IPV4*none***"
ICCID_894931_1="DEU*Sipgate*sipgate*IPV4*none***"
ICCID_894922_1="DEU*O2 Germany*internet*IPV4*none***"
ICCID_894921_1="DEU*Aldi Talk*internet.eplus.de*IPV4*PAP*eplus*eplus*"
ICCID_894920_1="DEU*Gigacube*home.vodafone.de*IPV4*none***"
ICCID_894920_2="DEU*Vodafone *web.vodafone.de*IPV4*CHAP*vodafone*vodafone*"
ICCID_894902_1="DEU*Telekom DEU*internet.telekom*IPV4*none***"
ICCID_894817_1="POL*Aero2*darmowy*IPV4*none***"
ICCID_894812_1="POL*Cyfrowy Polsat*internet.cp*IPV4*none***"
ICCID_894806_1="POL*Play Internet*internet*IPV4*none***"
ICCID_894803_1="POL*Orange Polska*internet*IPV4*PAP*internet*internet*"
ICCID_894802_1="POL*T-Mobile Polska*internet*IPV4*none***"
ICCID_894801_1="POL*Plus *internet*IPV4*none***"
ICCID_894801_2="POL*Plus Internet*internet*IPV4*none***"
ICCID_894780_1="NOR*VODA*data*IPV4*none***"
ICCID_894708_1="NOR*Telia No*telia*IPV4*none***"
ICCID_894707_1="NOR*ice.net*internet*IPV4*none***"
ICCID_894701_1="NOR*TELENOR*telenor*IPV4*none***"
ICCID_894647_1="SWE*VODA*internet*IPV4*none***"
ICCID_894627_1="SWE*Foggmobile*data.tre.se*IPV4*none***"
ICCID_894620_1="SWE*COMVIQ GSM*4g.tele2.se*IPV4*none***"
ICCID_894617_1="SWE*WiFog*data.tre.se*IPV4*none***"
ICCID_894614_1="SWE*DK TDC*internet.se*IPV4*none***"
ICCID_8946100_1="SWE*Tele 2 AB*internet.telenor.se*IPV4*none***"
ICCID_894608_1="SWE*Telenor*internet.telenor.se*IPV4*none***"
ICCID_894607_1="SWE*3 SE*bredband.tre.se*IPV4*none***"
ICCID_8946012_1="SWE*Telia SE*online.telia.com*IPV4*none***"
ICCID_894601_1="SWE*Telia SE*online.telia.se*IPV4*none***"
ICCID_894601_2="SWE*Halebop*halebop.telia.se*IPV4*none***"
ICCID_894506_1="DNK*3 DK*bredband.tre.dk*IPV4*none***"
ICCID_894506_2="DNK*Oister*bredband.oister.dk*IPV4*none***"
ICCID_894504_1="DNK*Telia DK*www.internet.mtelia.dk*IPV4*none***"
ICCID_894502_1="DNK*Telenor DK*Internet*IPV4*none***"
ICCID_894501_1="DNK*TDC Mobil*internet*IPV4*none***"
ICCID_894455_1="GBR*Sure*internet*IPV4*none***"
ICCID_894450_1="GBR*JT Group*pepper*IPV4*none***"
ICCID_894450_2="GBR*Truphone*truphone.com*IPV4*none***"
ICCID_8944433_1="GBR*Virgin Media*goto.virginmobile.uk*IPV4*PAP*user**"
ICCID_894430_1="GBR*T-Mobile*everywhere*IPV4*none***"
ICCID_894430_2="GBR*Virgin Media*goto.virginmobile.uk*IPV4*PAP*user**"
ICCID_894430_3="GBR*EE*everywhere*IPV4*none***"
ICCID_894420_1="GBR*Globalgig*mbb.voiamo.net*IPV4****"
ICCID_894420_2="GBR*3 UK*3internet*IPV4*none***"
ICCID_894420_3="GBR*3 UK*three.co.uk*IPV4*none***"
ICCID_894412_1="GBR*Orange*everywhere*IPV4*none***"
ICCID_894411_1="GBR*O2 UK*mobile.o2.co.uk*IPV4*PAP*o2web*password*"
ICCID_894411_2="GBR*Giff Gaff*giffgaff.com*IPV4*PAP*giffgaff**"
ICCID_894411_3="GBR*O2 Pay and Go*payandgo.o2.co.uk*IPV4*PAP*web*web*"
ICCID_894410_1="GBR*Vodafone*internet*IPV4*PAP*web*web*"
ICCID_894410_2="GBR*Vodafone_pp*pp.internet*IPV4*PAP*web*web*"
ICCID_894410_3="GBR*Talkmobile*talkmobile.co.uk*IPV4*PAP*wap*wap*"
ICCID_894410_4="GBR*Lebara*uk.lebara.mobi*IPV4*PAP*wap*wap*"
ICCID_894410_5="GBR*voda UK*talkmobile.co.uk*IPV4****"
ICCID_894403_1="GBR*Airtel-Vodafone*airtel-ci-gprs.com*IPV4*none***"
ICCID_894403_2="GBR*Jersey Telenet*airtel-ci-gprs.com*IPV4*none***"
ICCID_894400_1="GBR*BT Mobile*btmobile.bt.com*IPV4*PAP*bt*bt*"
ICCID_894318_1="AUT*3 AT*drei.at*IPV4*none***"
ICCID_894310_1="AUT*3 AT*drei.at*IPV4*none***"
ICCID_894307_1="AUT*HoT Internet*webaut*IPV4*none***"
ICCID_894307_2="AUT*Tele-ring*web *IPV4*PAP*web@telering.at*web*"
ICCID_894305_1="AUT*Orange A*orange.web*IPV4*PAP*web*web*"
ICCID_894303_1="AUT*T-Mobile AUT*gprsinternet*IPV4*PAP*t-mobile*tm*"
ICCID_894301_1="AUT*A1*a1.net*IPV4*PAP*ppp@a1plus.at*ppp*"
ICCID_894301_2="AUT*Aon*aon.data*IPV4*none***"
ICCID_8942702_1="QAT*Vodafone Qatar*web.vodafone.com.qa*IPV4*none***"
ICCID_8942306_1="LIE*Orange*mtoom.com*IPV4*none***"
ICCID_8942305_1="LIE*FL1*mobilkom.li*IPV4*PAP*gprs@a1plus.at*mobilkom*"
ICCID_8942103_1="SVK*Swan SK*internet*IPV4*none***"
ICCID_8942102_1="SVK*Telekom SK*internet*IPV4*none***"
ICCID_8942101_1="SVK*Orange*internet*IPV4*PAP*wap*wap*"
ICCID_8942031_1="CZE*Vodafone CZ*internet*IPV4*none***"
ICCID_8942020_1="CZE*ET - CZ*internet*IPV4*none***"
ICCID_8942001_1="CZE*T-Mobile CZ*internet.t-mobile.cz*IPV4*PAP*wap*wap*"
ICCID_894103_1="CHE*Salt*internet*IPV4*none***"
ICCID_894103_2="CHE*Salt. PrePay*click*IPV4*none***"
ICCID_894103_3="CHE*Orange*internet*IPV4*none***"
ICCID_894102_1="CHE*Sunrise*internet*IPV4*none***"
ICCID_894101_1="CHE*Swisscom *gprs.swisscom.ch*IPV4*none***"
ICCID_894100_1="CHE*UFONE*data*IPV4*none***"
ICCID_894100_2="CHE*Telenor*telenor.com*IPV4*none***"
ICCID_894100_3="CHE*WaridTel*Wap.warid*IPV4*none***"
ICCID_894010_1="ROU*Orange*internet*IPV4*PAP*internet*orange*"
ICCID_894005_1="ROU*Digi*Internet*IPV4*none***"
ICCID_894001_1="ROU*Vodafone*internet.vodafone.ro*IPV4*PAP*internet.vodafone.ro*vodafone*"
ICCID_8939999_1="ITA*Tre*tre.it*IPV4*none***"
ICCID_893988_1="ITA*Wind*internet.wind*IPV4*none***"
ICCID_8939500_1="ITA*Iliad*Iliad*IPV4*none***"
ICCID_8939361_1="ITA*Digi*digi.mobile*IPV4*none***"
ICCID_8939109_1="ITA*Ho*web.ho-mobile.it*IPV4*none*campo vuoto*campo vuoto*"
ICCID_8939103_1="ITA*voda ITA phone*mobile.vodafone.it*IPV4*none***"
ICCID_8939104_1="ITA*voda ITA data*web.omnitel.it*IPV4*none***"
ICCID_8939080_1="ITA*FW WEB MOBILE*apn.fastweb.it*IPV4*none***"
ICCID_8939010_1="ITA*Tiscali*tiscalimobileinternet*IPV4*none***"
ICCID_8939010_2="ITA*TIM*ibox.tim.it*IPV4*none***"
ICCID_8939010_3="ITA*CoopVoce*web.coopvoce.it*IPV4*none***"
ICCID_8939019_1="ITA*CoopVoce*web.coopvoce.it*IPV4*none***"
ICCID_893953_1="ITA*CoopVoce*web.coopvoce.it*IPV4*none***"
ICCID_893933_1="ITA*PosteMobile*wap.postemobile.it*IPV4*none***"
ICCID_8939079_1="ITA*Kena*web.kenamobile.it*IPV4*none***"
ICCID_8938903_1="MKD*Vip MKD*vipoperator*IPV4*PAP*vipoperator*vipoperator*"
ICCID_8938901_1="MKD*T-Mobile*internet*IPV4*PAP*internet*t-mobile*"
ICCID_8938705_1="BIH*Mtel*Mtelsmart*IPV4*none***"
ICCID_8938670_1="SLO*TUS*internet*IPV4*none***"
ICCID_8938641_1="SLO*Mobitel*internet*IPV4*none***"
ICCID_8938640_1="SLO*Si.mobil*internet.simobil.si*IPV4*none***"
ICCID_8938599_1="COR*T-Mobile HR*web.htgprs*IPV4*none***"
ICCID_8938597_1="COR*T-Mobile HR*web.htgprs*IPV4*none***"
ICCID_8938591_1="COR*VIPnet*data.vip.hr*IPV4*none***"
ICCID_8938502_1="COR*TELE2*wap.tele2.hr*IPV4*none***"
ICCID_8938201_1="MNE*Telenor*www.telenor.me*IPV4*none***"
ICCID_8938200_1="MNE*T-Mobile*internet-postpaid*IPV4*none***"
ICCID_8938105_1="SRB*Vip*vipmobile*IPV4*PAP*vipmobile*vipmobile*"
ICCID_8938103_1="SRB*MTS*gprsinternet*IPV4*PAP*mts*064*"
ICCID_8938101_1="SRB*Telenor*internet*IPV4*PAP*telenor*gprs*"
ICCID_8938007_1="UKR*Utel*3g.utel.ua*IPV4*none***"
ICCID_8938006_1="UKR*Life*internet*IPV4*none***"
ICCID_8938003_1="UKR*KYIVSTAR*www.ab.kyivstar.net*IPV4*none***"
ICCID_8938001_1="UKR*UMC*internet*IPV4*none***"
ICCID_8937710_1="MON*MONACO*internet*IPV4*none***"
ICCID_8937701_1="MON*vala*vala*IPV4*none***"
ICCID_8937700_1="MON*VODA*data*IPV4*none***"
ICCID_8937603_1="AND*Andorra Telecom*internetand*IPV4*none***"
ICCID_8937503_1="BLR*Life*internet.life.com.by*IPV4*none***"
ICCID_8937502_1="BLR*MTS*mts*IPV4*PAP*mts*mts*"
ICCID_8937501_1="BLR*VELCOM*data*IPV4*none***"
ICCID_8937493_1="ARM*Vivacell*inet.vivacell.am*IPV4*none***"
ICCID_8937450_1="ARM*VODA*internet*IPV4*none***"
ICCID_8937303_1="MDA*UNITE*internet3g.unite.md*IPV4*none***"
ICCID_8937302_1="MDA*MOLDDCELL*internet*IPV4*none***"
ICCID_8937301_1="MDA*Orange*wap.orange.md*IPV4*none***"
ICCID_8937203_1="EST*Tele2*internet.tele2.ee*IPV4*none***"
ICCID_8937202_1="EST*Elisa Eesti*internet*IPV4*none***"
ICCID_8937201_1="EST*EMT*internet.emt.ee*IPV4*none***"
ICCID_8937105_1="LAT*BITE*internet*IPV4*none***"
ICCID_8937102_1="LAT*Tele2*internet.tele2.lv*IPV4*none***"
ICCID_8937101_1="LAT*LMT *internet.lmt.lv*IPV4*none***"
ICCID_8937003_1="LTU*TELE2*internet.tele2.lt*IPV4*none***"
ICCID_8937002_1="LTU*BITE GSM*wap.biteplius.lt*IPV4*none***"
ICCID_8937001_1="LTU*OMNITEL*omnitel*IPV4*PAP*omni*omni*"
ICCID_8937000_1="LTU*Mezon*mezon*IPV4*none***"
ICCID_893670_1="HUN*voda*internet.vodafone.net*IPV4*none***"
ICCID_893630_1="HUN*TMO H*internet.telekom*IPV4*none***"
ICCID_893620_1="HUN*pannon*net*IPV4*none***"
ICCID_8935905_1="BUL*GLOBUL*telnor*IPV4*CHAP*globul*globul*"
ICCID_8935903_1="BUL*vivatel*internet.vivatel.bg*IPV4*PAP*vivatel*vivatel*"
ICCID_8935901_1="BUL*M-Tel*inet-gprs.mtel.bg*IPV4*none***"
ICCID_89358091_1="FIN*Saunalahti*internet*IPV4*none***"
ICCID_89358090_1="FIN*Saunalahti*internet.saunalahti*IPV4*none***"
ICCID_8935806_1="FIN*DNA LTD*data.dna.fi*IPV4*none***"
ICCID_8935804_1="FIN*DNA LTD*data.dna.fi*IPV4*none***"
ICCID_8935743_1="CYP*MTN*internet*IPV4*none***"
ICCID_8935720_1="CYP*PrimeTel*ip.primetel*IPV4*none***"
ICCID_8935701_1="CYP*CytaVoda*internet*IPV4*none***"
ICCID_8935677_1="MLT*3GT*web.melita*IPV4*none***"
ICCID_8935610_1="MLT*gomobile*gosurfing*IPV4*none***"
ICCID_8935601_1="MLT*vodafone*internet*IPV4*PAP*internet*internet*"
ICCID_8935502_1="ALB*vodafone*vodafoneweb*IPV4*none***"
ICCID_8935501_1="ALB*AMC*internet.amc*IPV4*none***"
ICCID_8935411_1="ISL*NOVA*internet.nova.is*IPV4*none***"
ICCID_8935402_1="ISL*Vodafone*vmc.gprs.is*IPV4*none***"
ICCID_8935401_1="ISL*Siminn*wap.simi.is*IPV4*none***"
ICCID_8935311_1="IRL*Tesco Mobile*tescomobile.liffeytelecom.com*IPV4*none***"
ICCID_8935305_1="IRL*Hutchison 3G*3internet*IPV4*none***"
ICCID_8935303_1="IRL*eir*data.myeirmobile.ie*IPV4*none***"
ICCID_8935302_1="IRL*o2 IRL*internet*IPV4*none***"
ICCID_8935301_1="IRL*vodafone*hs.vodafone.ie*IPV4*PAP*vodafone*vodafone*"
ICCID_8935299_1="LUX*VOXmobile*vox.lu*IPV4*none***"
ICCID_8935277_1="LUX*TANGO*internet*IPV4*PAP*tango*tango*"
ICCID_8935201_1="LUX*LUXGSM*web.pt.lu*IPV4*none***"
ICCID_8935106_1="PRT*MEO*internet*IPV4*none***"
ICCID_8935103_1="PRT*OPTIM*internet*IPV4*none***"
ICCID_8935101_1="PRT*Vodafone*internet.vodafone.pt*IPV4*none***"
ICCID_8935101_2="PRT*Vodafone*net2.vodafone.pt*IPV4*none***"
ICCID_893490_1="ESP*Orange*inet.es*IPV4*none***"
ICCID_8934568_1="ESP*Vodafone*airtelwap.es*IPV4*none***"
ICCID_893456_1="ESP*Vodafone*ac.vodafone.es*IPV4*none***"
ICCID_893434_1="ESP*Aire*apn.operador.es*IPV4*none***"
ICCID_893407_1="ESP*Movistar*movistar.es*IPV4*PAP*MOVISTAR*MOVISTAR*"
ICCID_893404_1="ESP*Yoigo*internet*IPV4*none***"
ICCID_893401_1="ESP*Orange*internet*IPV4*PAP*orange*orange*"
ICCID_893401_2="ESP*Orange Simyo*orangeworld*IPV4*PAP*orange*orange*"
ICCID_893329_1="FRA*Virgin Mobile*ofnew.fr*IPV4*none*orange*orange*"
ICCID_893326_1="FRA*NRJ Mobile*fnetnrj*IPV4*none***"
ICCID_893323_1="FRA*SFR*sl2sfr*IPV4*none***"
ICCID_8933103_1="FRA*SFR*sl2sfr*IPV4*none***"
ICCID_893320_1="FRA*Bouygues Tel*faiebouygtel*IPV4*none***"
ICCID_893320_2="FRA*Lebara France*fr.lebara.mobi*IPV4*none***"
ICCID_893315_1="FRA*Free Mobile*free*IPV4*none***"
ICCID_893310_1="FRA*SFR Modem*websfr*IPV4*none***"
ICCID_8933018_1="FRA*Orange France*orange*IPV4*PAP*orange*orange*"
ICCID_893301_1="FRA*Orange France*orange.fr*IPV4*PAP*orange*orange*"
ICCID_893301_2="FRA*Enterprise One*internet-entreprise*IPV4*PAP*orange*orange*"
ICCID_893203_1="BEL*Telenet*gprs.base.be*IPV4*none*base*base*"
ICCID_893202_1="BEL*mobi**mworld.be*IPV4*none***"
ICCID_893200_1="BEL*Proximus*internet.proximus.be*IPV4*none***"
ICCID_893142_1="NLD*Tele2*internet.tele2.nl*IPV4*none***"
ICCID_893116_1="NLD*T-Mobile NL*internet*IPV4*none***"
ICCID_893110_1="NLD*Ericsson*256qam.ki*IPV4*none***"
ICCID_893110_2="NLD*Chalmers*internet.erinet.se*IPV4*none***"
ICCID_893010_1="GRC*Wind Hellas*gint.b-online.gr*IPV4*none***"
ICCID_893005_1="GRC*Vodafone*internet*IPV4*none***"
ICCID_893001_1="GRC*Cosmote*internet*IPV4*none***"
ICCID_8929801_1="FAI*FT-GSM*internet*IPV4*none***"
ICCID_8929702_1="ARU*Digicel*web.digicelaruba.com*IPV4*none***"
ICCID_892745_1="ZAF*VODA*internet*IPV4*none***"
ICCID_892707_1="ZAF*Cell C*internet*IPV4*none***"
ICCID_892702_1="ZAF*Telkom SA Ltd*internet*IPV4*none***"
ICCID_892700_1="ZAF*MTN-SA*internet*IPV4*none***"
ICCID_8926902_1="COM*TELMA*www*IPV4*none***"
ICCID_8926702_1="BOT*Orange*orangeworld.orange.co.bw*IPV4*none***"
ICCID_8926502_1="MAW*AIRTEL*internet*IPV4*none***"
ICCID_8926501_1="MAW*TNM*internet*IPV4*none***"
ICCID_8926403_1="NAM*Leo*tn*IPV4*none***"
ICCID_8926304_1="ZIM*ECONET*econet.net*IPV4*none***"
ICCID_8926203_1="RUN*Free*free.re*IPV4*none***"
ICCID_8926200_1="RUN*Orange*orangerun.entreprise*IPV4*none***"
ICCID_8926104_1="MAD*TELMA*internet*IPV4*none***"
ICCID_8926102_1="MAD*Orange*orangenet*IPV4*none***"
ICCID_8926003_1="ZAM*Zamtel*Zamtel.net*IPV4*none***"
ICCID_8925804_1="MOZ*VodaCom*flexi-internet2*IPV4*none***"
ICCID_8925633_1="UGA*Smilecoms*internet*IPV4*none***"
ICCID_8925614_1="UGA*Orange*orange.ug*IPV4*none***"
ICCID_8925610_1="UGA*MTN*yellopix.mtn.co.ug*IPV4*none***"
ICCID_8925601_1="UGA*AIRTEL*web.ug.zain.com*IPV4*none***"
ICCID_8925511_1="TAN*internet*internet*IPV4*none***"
ICCID_8925504_1="TAN*Vodacom*internet*IPV4*none***"
ICCID_8925503_1="TAN*Zantel*znet*IPV4*none***"
ICCID_8925502_1="TAN*TIGO*tigoweb*IPV4*none***"
ICCID_8925500_1="TAN*TTCL*internet*IPV4*none***"
ICCID_8925407_1="KEN*Orange*bew.orange.co.ke*IPV4*none***"
ICCID_8925403_1="KEN*Airtel*internet*IPV4*none***"
ICCID_8925402_1="KEN*Safaricom*safaricom*IPV4*none***"
ICCID_8925271_1="SOM*SOMTEL*somtel*IPV4*none***"
ICCID_8925101_1="ETH*ETC*etc.com*IPV4*none***"
ICCID_8925017_1="RWA*MTN*internet.mtn*IPV4*none***"
ICCID_8925014_1="RWA*Airtel*internet*IPV4*none***"
ICCID_8925013_1="RWA*TIGO*web.tigo.rw*IPV4*none***"
ICCID_8925010_1="RWA*RCELL*internet*IPV4*none***"
ICCID_8924901_1="SUD*MobiTel*internet*IPV4*none***"
ICCID_8924810_1="SEY*Telecom Airtel*airtelgprs.com*IPV4*none***"
ICCID_8924801_1="SEY*Cable & Wireless*internet*IPV4*none***"
ICCID_8924404_1="ANG*MOVICEL*internet.movicel.co.ao*IPV4*none***"
ICCID_8924402_1="ANG*UNITEL*internet.unitel.co.ao*IPV4*none***"
ICCID_8924302_1="COD*CELTEL*internet*IPV4*none***"
ICCID_8924301_1="COD*VODACOM*vodanet*IPV4*PAP*vodalive**"
ICCID_8924210_1="CGO*MTN*internet.mtn.cg*IPV4*none***"
ICCID_8924201_1="CGO*CELTEL*internet.airtel.cg.com*IPV4*none***"
ICCID_8924103_1="GAB*AIRTEL*internet.ga.airtel.com*IPV4*none***"
ICCID_8924101_1="GAB*LIBERTIS*Libertis*IPV4*none***"
ICCID_8923802_1="CPV*T+*default*IPV4*none***"
ICCID_8923702_1="CMR*Orange*orangecmgprs*IPV4*PAP***"
ICCID_8923700_1="CMR*MTN*mtnwap*IPV4*none***"
ICCID_8923503_1="CHA*TIGO*internet*IPV4*none***"
ICCID_8923501_1="CHA*CELTEL*airtel*IPV4*none***"
ICCID_8923451_1="NGR*Sweden3G*relish.apn*IPV4*none***"
ICCID_8923450_1="NGR*glo*gloflat*IPV4*PAP*flat*flat*"
ICCID_8923427_1="NGR*Smilecoms*internet*IPV4*none***"
ICCID_8923401_1="NGR*MTN*data*IPV4*none***"
ICCID_8923400_1="NGR*Etisalat*etisalat*IPV4*none***"
ICCID_8923308_1="GHA*Surfline*internet*IPV4*none***"
ICCID_8923306_1="GHA*AIRTEL*data*IPV4*none***"
ICCID_8923302_1="GHA*ONEtouch*internet*IPV4*none***"
ICCID_8923301_1="GHA*MTN*internet*IPV4*none***"
ICCID_8923002_1="MRI*Orange*Orange*IPV4*none***"
ICCID_8922903_1="BEN*MTN*internet.mtn.bj*IPV4*none***"
ICCID_8922902_1="BEN*Moov*moov*IPV4*none***"
ICCID_8922803_1="TOG*MOOV*moov*IPV4*none***"
ICCID_8922704_1="NIG*Orange*orange.ne*IPV4*none***"
ICCID_8922703_1="NIG*MOOV*moov*IPV4*none***"
ICCID_8922602_1="BUR*celtel*internet.bf.celtel.com*IPV4*none***"
ICCID_8922601_1="BUR*Telmob*data*IPV4*none***"
ICCID_8922505_1="CIV*MTN*web.mtn.ci*IPV4*none***"
ICCID_8922503_1="CIV*Orange*orangecidata*IPV4*none***"
ICCID_8922502_1="CIV*Telecel*moov*IPV4*none***"
ICCID_8922404_1="GUI*areeba*internet.mtn.com.gn*IPV4*none***"
ICCID_8922401_1="GUI*Orange*internet*IPV4*none***"
ICCID_8922302_1="MLI*Orange*data*IPV4*none***"
ICCID_8922301_1="MLI*MALITEL*internet*IPV4*none***"
ICCID_8922202_1="MTN*EMR*data*IPV4*none***"
ICCID_8922201_1="MTN*MATTEL*data*IPV4*none***"
ICCID_8922101_1="SEN*Orange*orange.sn*IPV4*none***"
ICCID_8921602_1="TUN*Tunisie Telecom*internet.tn*IPV4*PAP*gprs*gprs*"
ICCID_8921601_1="TUN*Orange*weborange*IPV4*none***"
ICCID_8921303_1="ALG*NEDJMA*web*IPV4*PAP*nedjma*nedjma*"
ICCID_8921301_1="ALG*Mobilis*internet*IPV4*PAP*internet*internet*"
ICCID_8921202_1="MAR*INWI*www.wana.ma*IPV4*none***"
ICCID_8921201_1="MAR*IAM*wap.iamgprs.ma*IPV4*none***"
ICCID_8921200_1="MAR*MOR MEDITEL*internet1.meditel.ma*IPV4*PAP*medinet*medinet*"
ICCID_8921138_1="SSD*RACK 4 /75*internet.dynamic.com*IPV4*none***"
ICCID_8921102_1="SSD*MTN*internet*IPV4*none***"
ICCID_892003_1="EGY*etisalat*etisalat*IPV4*none***"
ICCID_892002_1="EGY*Vodafone*internet.vodafone.net*IPV4*PAP*internet*internet*"
ICCID_892001_1="EGY*MobiNiL*mobinilweb*IPV4*none***"
ICCID_8914800_1="USA*Verizon*vzwinternet*IPV4*none***"
ICCID_8914800_2="USA*Visible*vsblinternet*IPV4*none***"
ICCID_8914800_3="USA*Xfinity*COMCAST.RSLR.VZWENTP*IPV4*none***"
ICCID_891310_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891270_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891260_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891250_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891240_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891230_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891220_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_891210_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_8901110_1="USA*Claro*fast.t-mobile.com*IPV4V6*none***"
ICCID_890123_1="USA*C Spire*internet.cs4glte.com*IPV4V6*none***"
ICCID_8901320_1="USA*Cellular One*isp.cellularoneaz.net*IPV4*none***"
ICCID_890207_1="BEL*Telenet*mobile.internet.be*IPV4*none***"
ICCID_8901580_1="USA*US Cellular*usccinternet*IPV4V6*none***"
ICCID_890126_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_890126_2="USA*Mint Mobile*Wholesale*IPV4V6*none***"
ICCID_890126_3="USA*Google Fi*h2g2*IPV4V6*none***"
ICCID_890126_4="USA*T Mobile*fbb.home*IPV4V6*none***"
ICCID_890124_1="USA*T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_89302720_1="CAD*Rogers*lteinternet.apn*IPV4*none***"
ICCID_89302610_1="CAD*Bell*inet.bell.ca*IPV4V6*none***"
ICCID_89302610_2="CAD*Bell*pda.bell.ca*IPV4*none***"
ICCID_8985219_1="HKG*csl.*mobile*IPV4*none***"
ICCID_8985216_1="HKG*csl.*mobile*IPV4*none***"
ICCID_8985210_1="HKG*csl.*mobile*IPV4*none***"
ICCID_8985204_1="HKG*3 HK LTE*mobile.lte.three.com.hk*IPV4*none***"
ICCID_8985204_2="HKG*3 HK 3G*mobile.three.com.hk*IPV4*none***"
ICCID_8985203_1="HKG*3 HK LTE*mobile.lte.three.com.hk*IPV4*none***"
ICCID_8985203_2="HKG*3 HK 3G*mobile.three.com.hk*IPV4*none***"
ICCID_8935802_1="FIN*Elisa Corp*internet*IPV4*none***"
ICCID_8935801_1="FIN*Sonera*internet*IPV4*none***"
ICCID_8931041_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8931038_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8931017_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8931015_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8901438_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8901410_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8901410_2="USA*AT&T Mobile*phone*IPV4V6*none***"
ICCID_8901280_1="USA*AT&T Mobile*fast.t-mobile.com*IPV4V6*none***"
ICCID_8901150_1="USA*AT&T Mobile*Broadband*IPV4V6*none***"
ICCID_8901150_2="USA*Cricket mht*mht*IPV4*none***"
ICCID_8901150_3="USA*Cricket ndo*ndo*IPV4*none***"
ICCID_8901150_4="USA*Cricket Wireless*ndo*IPV4*none***"
ICCID_8901170_1="USA*Unreal Mobile*unreal.com.attz*IPV4*none***"
ICCID_8901170_2="USA*Freedompop*fp.com.attz*IPV4*none***"
ICCID_896103_1="AUS*Vodafone*live.vodafone.com*IPV4*none***"
ICCID_896102_1="AUS*Optus*connect*IPV4*none***"
ICCID_894705_1="NOR*Network Norway*internet*IPV4*none***"
ICCID_894705_2="NOR*One Call*internet*IPV4*none***"
ICCID_894704_1="NOR*Mobile Norway*internet*IPV4*none***"
ICCID_894700_1="NOR*TELENOR*telenor*IPV4*none***"
ICCID_893457_1="ESP*Lowi*lowi.private.omv.es*IPV4*none***"
ICCID_893425_1="ESP*InternetR*internet.mundo-r.com*IPV4*none***"
ICCID_893421_1="ESP*Jazztel*jazzinternet*IPV4*none***"
ICCID_893419_1="ESP*Simyo*gprs-service.com*IPV4*none***"
ICCID_893407_1="ESP*Pepephone*gprs.pepephone.com*IPV4*none***"
ICCID_893403_1="ESP*Tuenti*tuenti.com*IPV4*PAP*tuenti*tuenti*"
ICCID_893144_1="NLD*Vodafone Lib*office.vodafone.nl*IPV4*CHAP*vodafone*vodafone*"
ICCID_8931440_1="NLD*Vodafone Lib*live.vodafone.com*IPV4*none***"
ICCID_893126_1="NLD*Telfort B.V.*internet*IPV4*none***"
ICCID_893108_1="NLD*KPN B.V.*fastinternet*IPV4*none***"
ICCID_891223_1="CAD*TELUS ISP*isp.telus.com*IPV4*none***"

echo 0 >/tmp/mobile_ping_result

value_mapping() #$1: country, $2: pdptype. $3: authtype
{
	case "$1" in
		AUT)
			country="Austria"
			;;
		BEL)
			country="Belgium"
			;;
		CZE)
			country="Czech"
			;;
		FIN)
			country="Finland"
			;;
		FRA)
			country="France"
			;;
		DEU)
			country="Germany"
			;;
		HUN)
			country="Hungary"
			;;
		ITA)
			country="Italy"
			;;
		NLD)
			country="Netherlands"
			;;
		NOR)
			country="Norway"
			;;
		NZL)
			country="New Zealand"
			;;
		POL)
			country="Poland"
			;;
		PRT)
			country="Portugal"
			;;
		SWE)
			country="Sweden"
			;;
		CHE)
			country="Switzerland"
			;;
		SVK)
			country="Slovakia"
			;;
		USA)
			country="USA"
			;;
		*)
			echo "[MB_Scan]: Current Country is $1, set it as other."
			country="other"
			isp="other"
			;;
	esac

	case "$2" in
		IPV4)
			pdptype="0"
			pdptype_opt="-4"
			;;
		PPP)
			pdptype="1"
			pdptype_opt="-4"
			;;
		IPV6)
			pdptype="2"
			pdptype_opt="-6"
			;;
		IPV4V6)
			pdptype="3"
			pdptype_opt="-4 -6"
			;;
		*)
			echo "[MB_Scan]: Incorrect PDP Type=$2 in Mobile Scan."
			;;
	esac

	case "$3" in
		none)
			authtype="0"
			;;
		PAP)
			authtype="1"
			;;
		CHAP)
			authtype="2"
			;;
		MSCHAPV2)
			authtype="3"
			;;
		*)
			echo "[MB_Scan]: Incorrect Authtype=$3 in Mobile Scan."
			;;
	esac

}

quectel_option_check() # $1:apn, $2:username, $3:password
{
	param_s=""
	[ -n "$1" ] && param_s="$1" || return
	[ "x$2" != "xnone" ] && param_s="$param_s $2" || return
	[ "x$3" != "xnone" ] && param_s="$param_s $3"
}

mobile_scan()
{
	current_full_iccid=$(mbctrl.sh --get-iccid)
	iccid_3=${current_full_iccid:0:6}
	iccid_2=${current_full_iccid:0:7}
	iccid_1=${current_full_iccid:0:8}

	current_iccid=""

	echo "[MB_Scan]: Full ICCID=$current_full_iccid."

	for m in $(seq 1 3)
	do
		conf=""
		eval iccid_tmp=\${iccid_$m}
		eval conf=\${ICCID_${iccid_tmp}_1}
		if [ -n "$conf" ]; then
			current_iccid=$iccid_tmp
			echo "[MB_Scan]: Current ICCID=$current_iccid."
			break
		fi
	done

	n=1
	while true
	do
		killall quectel-CM
		local vzw_opt

		eval conf=\${ICCID_${current_iccid}_$n}
		[ -z "$conf" ] && echo "[MB_Scan]: FULL ICCID can not match anyone of the database." && break

		echo "[MB_Scan]: current config setting is ICCID_${current_iccid}_$n=$conf"

		if [ "x${current_iccid}" = "x893202" ]; then
			country=BEL
			isp="mobi*"
			apn="mworld.be"
			pdptype="IPV4"
			authtype="none"
			username=""
			password=""
		else
			country=`echo $conf|cut -d "*" -f1`
			isp=`echo $conf|cut -d "*" -f2`
			apn=`echo $conf|cut -d "*" -f3`
			pdptype=`echo $conf|cut -d "*" -f4`
			authtype=`echo $conf|cut -d "*" -f5`
			username=`echo $conf|cut -d "*" -f6`
			password=`echo $conf|cut -d "*" -f7`
		fi

		value_mapping $country $pdptype $authtype

		pincode=`$CONFIG get wan2.pin_code`

		if [ "x${current_iccid}" = "x8914800" ]; then
			vzw_opt="3"
		fi

		echo "[MB_Scan]: current config setting=country-$country,isp-$isp,apn-$apn,pdptype-$pdptype,authtype-$authtype,username-$username,password-$password,pincode-$pincode."

		quectel_option_check $apn ${username:-"none"} ${password:-"none"}
		quectel-CM ${vzw_opt:+-n 3} ${param_s:+-s $param_s} ${authtype:+-a $authtype} ${pincode:+-p $pincode} ${pdptype_opt} &

		sleep 20	#wait for setting wan ip over

		ping -c 5 8.8.8.8 >/tmp/mobile_ping
		if [ -n "`sed -n '/ttl/p' /tmp/mobile_ping`" ]; then
			echo "[MB_Scan]: Mobile Scan Ping Success!"
			echo 1 > /tmp/mobile_ping_result
			$CONFIG set wan2.country="$country"
			$CONFIG set wan2.isp="$isp"
			$CONFIG set wan2.apn="$apn"
			$CONFIG set wan2.pdp_type="$pdptype"
			$CONFIG set wan2.authtype="$authtype"
			$CONFIG set wan2.user_name="$username"
			$CONFIG set wan2.password="$password"

			$CONFIG commit
			break
		else
			echo "[MB_Scan]: Mobile Scan Ping Fail!"
			echo 0 > /tmp/mobile_ping_result
		fi

		n=$(($n+1))
	done
}

usage()
{
	printf "%s\n" \
		"Usage: dni_mobile_scan.sh start"
	exit 1
}

case $1 in
	"start")
		mobile_scan
		;;
	*)
		usage ;;
esac
