lib:dialog{label="ͼ ��";
:row{
:column{
:list_box{label="ͼ�����";height=21;width=15;key="ls";}
:button{label="�������"; key="add_ls";}
:button{label="ɾ�����"; key="del_ls";}
:text{label="";key="t1";alignment=centered;}
//:edit_box{label="����:"; width= 18 ; key="scale"; value = "1";}
:row{
spacer;
:toggle{label=" ը  ��";key="expl";value="1";}
spacer;
}
}
:column{
:row{
:image_button{height=8;width=25;color=0;key="icon1";}
:image_button{height=8;width=25;color=0;key="icon2";}
:image_button{height=8;width=25;color=0;key="icon3";}
:image_button{height=8;width=25;color=0;key="icon4";}
}
:row{
:image_button{height=8;width=25;color=0;key="icon5";}
:image_button{height=8;width=25;color=0;key="icon6";}
:image_button{height=8;width=25;color=0;key="icon7";}
:image_button{height=8;width=25;color=0;key="icon8";}
}
:row{
:image_button{height=8;width=25;color=0;key="icon9";}
:image_button{height=8;width=25;color=0;key="icon10";}
:image_button{height=8;width=25;color=0;key="icon11";}
:image_button{height=8;width=25;color=0;key="icon12";}
}
:row{
:image_button{height=8;width=25;color=0;key="icon13";}
:image_button{height=8;width=25;color=0;key="icon14";}
:image_button{height=8;width=25;color=0;key="icon15";}
:image_button{height=8;width=25;color=0;key="icon16";}
}
}
}
//spacer;
:text{label="";key="t2";alignment=centered;}
:row{
spacer;
:button{label="  ��  ��  "; key="accept";is_default=true;fixed_width=true;}
:button{label="  ��  ��  "; key="ch";fixed_width=true;}
:button{label="׷�ӱ�ע"; key="bz";fixed_width=true;}
:button{label="  ��  ��  "; key="zom";fixed_width=true;}
:button{label="  ɾ  ��  "; key="rem";fixed_width=true;}
:button{label="  ��һҳ  "; key="prev";fixed_width=true;}
:button{label="  ��һҳ  "; key="next";fixed_width=true;}
:button{label="  ��  ��  "; key="cancel";is_cancel=true;fixed_width=true;}
spacer;
}
spacer;
}


zoom:dialog{label="�õ�Ƭ�Ŵ�";
:image_button{color=0;key="lib_zoom";width=120;height=38;fixed_height=true;fixed_width=true;}
ok_only;
}


addlib:dialog{label="�������";
spacer;
:edit_box{label="ͼ�����:"; width= 18 ; key="addlib";}
spacer;
ok_only;
spacer;
}


addremark:dialog{label="׷�ӱ�ע";
spacer;
:edit_box{label="��ע��Ϣ:"; width= 25 ; key="rmk";}
spacer;
ok_only;
spacer;
}
