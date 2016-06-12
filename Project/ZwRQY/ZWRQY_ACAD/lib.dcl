lib:dialog{label="图 库";
:row{
:column{
:list_box{label="图库类别";height=21;width=15;key="ls";}
:button{label="增加类别"; key="add_ls";}
:button{label="删除类别"; key="del_ls";}
:text{label="";key="t1";alignment=centered;}
//:edit_box{label="比例:"; width= 18 ; key="scale"; value = "1";}
:row{
spacer;
:toggle{label=" 炸  开";key="expl";value="1";}
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
:button{label="  插  入  "; key="accept";is_default=true;fixed_width=true;}
:button{label="  变  更  "; key="ch";fixed_width=true;}
:button{label="追加备注"; key="bz";fixed_width=true;}
:button{label="  放  大  "; key="zom";fixed_width=true;}
:button{label="  删  除  "; key="rem";fixed_width=true;}
:button{label="  上一页  "; key="prev";fixed_width=true;}
:button{label="  下一页  "; key="next";fixed_width=true;}
:button{label="  退  出  "; key="cancel";is_cancel=true;fixed_width=true;}
spacer;
}
spacer;
}


zoom:dialog{label="幻灯片放大";
:image_button{color=0;key="lib_zoom";width=120;height=38;fixed_height=true;fixed_width=true;}
ok_only;
}


addlib:dialog{label="增加类别";
spacer;
:edit_box{label="图库类别:"; width= 18 ; key="addlib";}
spacer;
ok_only;
spacer;
}


addremark:dialog{label="追加备注";
spacer;
:edit_box{label="备注信息:"; width= 25 ; key="rmk";}
spacer;
ok_only;
spacer;
}
