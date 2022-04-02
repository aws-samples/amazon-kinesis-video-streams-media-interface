media demo使用方法说明
1、linux
- 确认Makefile中CHIP定义正确
- make menuconfig配置应用
- make
- 进入linux系统，加载相关驱动
- 运行./sample_vlcview ip

2、rtt
- make xxxx_defconfig
- make appconfig配置
- make all 生成media_demo.bin
- 加载media_demo.bin
- 设置ip
- msh命令行下输入vlcview -i ip_addr -p port

3、码流接收方法
- mjpeg
    http://board_ip:1111/?action=stream
- pes
    udp://@:2222（通道号根据channel依次加1）
- rtsp
    rtsp://board_ip:554/test.sdp（通道号根据channel依次加2）

目录说明
- components    出图相关组件，如码流打包发送、coolview连接等
- sample_common dsp、isp公共函数，如初始化ISP、创建编码通道等, overlay、venc、human_detect、motion detect及cover detect
- vlcview       基本出图流程
详见filelist.txt
