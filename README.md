基于STC12C5A60S2 + CDS5516 + nRF24L01 + LCD1602 + openmv等设计的舞蹈机器人。

整个项目包含一个主控台，三个舞蹈机器人；其中，一个机器人是主机器人（小青），其余两个机器人为从机器人（小粉、小蓝）。每个机器人均有17自由度，通过数字舵机精准控制实现整齐划一的高难度舞蹈动作。实现所需技术包含无线通信、数字舵机控制、语音交互、手势识别、机器人电量检测、LCD显示等功能。

三个机器人开机之后，为了实现舞蹈同步与机器人状态实时查看，在主控台使用按键开启/结束任务，并通过无线通信发送指令给主机器人；LCD2406液晶显示屏提示字幕可实时查看交互过程。

比赛开始控制台通过无线发送指令，可保证三个机器人同时跳舞。跳舞结束后，主机器人等待进行人机交互，主要包含语音交互和视觉交互。

歌曲使用《强军战歌》
                        
原文链接：https://blog.csdn.net/qq_44578108/article/details/140471377
