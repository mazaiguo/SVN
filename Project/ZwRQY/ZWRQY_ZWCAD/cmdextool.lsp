(setvar "cmdecho" 0)
(defun extcmd_err (msg)
  (cond ((= msg "Function cancelled") (princ))
        ((= msg "quit / exit abort") (princ))
	((= msg "停止 / 退出 终止") (princ))
	((= msg "取消函数") (princ))
	((= msg "函数取消") (princ))
	((= msg "取消") (princ))
	(t
	 (princ msg)
	 (princ) 
	)
  )
  (if oldos (setvar "osmode" oldos))
  (princ)
)
(setq *error* extcmd_err)



(defun ZwcadSetPath (tl_file_name)
  (setq tlpath (findfile "lib.dcl"))
  (setq tlpath (substr tlpath 1 (-(strlen tlpath) 7)))
  (strcat tlpath tl_file_name)
)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 图库 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  (defun c:lib ()
    (setvar "cmdecho" 0)
    (setvar "insunits" 0)

    (setq dcl (load_dialog "lib.dcl"))
    (new_dialog "lib" dcl)

    (libnewlist)

    (setq f (open (ZwcadSetPath "prt\\lib1.txt") "r"))
    (set_tile "ls" (read-line f))
    (setq page (atoi (read-line f)))
    (setq expl (read-line f))
    (set_tile "expl" expl)
    (close f)

    (extcmd_chgls)

	(if (= scale nil)
		(setq scale "1.0")
	)	
	(set_tile "scale" scale)
    (setq iconlist '("icon1"	 "icon2"     "icon3"	 "icon4"
		     "icon5"	 "icon6"     "icon7"	 "icon8"
		     "icon9"	 "icon10"    "icon11"	 "icon12"
		     "icon13"	 "icon14"    "icon15"	 "icon16")
    )
    (foreach icon iconlist (action_tile icon "(extcmd_sld_name $key)"))
    (action_tile "ls" "(extcmd_chgls)")
    (action_tile "add_ls" "(lib_add_ls)")
    (action_tile "del_ls" "(lib_del_ls)")
    
    (action_tile "ch" "(lib_save_var)(done_dialog 1)")
    (action_tile "bz" "(add_remark)")
    (action_tile "zom" "(extcmd_preview sldname)")
    (action_tile "rem" "(extcmd_remove)")
    (action_tile "expl" "(setq expl $value)")
    (action_tile "prev" "(setq page (1- page)) (showicon)")
    (action_tile "next" "(setq page (1+ page)) (showicon)")
    (action_tile "accept" "(lib_save_var)(done_dialog 2)")
    (action_tile "cancel" "(done_dialog 0)")
    ;;(action_tile "scale" "(setq scale $value)")
    (setq dd (start_dialog))
    (cond ((= dd 1)
	   (setq dwgname (strcat (ZwcadSetPath "prt\\") icna ".dwg"))

	   (setq p (getpoint "\n选取插入点:"))
	   (setq ss (ssget))
	   (if (findfile dwgname)
	     (vl-file-delete (findfile dwgname))
	   )
	   (command "-wblock" dwgname "" p ss "" "oops")

	   (setq sldname (strcat (ZwcadSetPath "prt\\") icna ".sld"))
	   (if (findfile sldname)
	     (vl-file-delete (findfile sldname))
	   )
           (setq oldpdm (getvar "pdmode"))
           (setq oldpds (getvar "pdsize"))
	   (command "color" 1 "pdmode" 35 "pdsize" -20 "point" p "color" "bylayer"
                    "mslide" sldname "erase" "l" "" "-purge" "b" "*" "n")
           (setvar "pdmode" oldpdm)
           (setvar "pdsize" oldpds)
	  )
	  ((= dd 2)
	   (setq dwg (findfile (strcat (ZwcadSetPath "prt\\") icna ".dwg")))
	   (if dwg
             ;;(while t
	       ;;(initget 7)
               ;;(setq p1 (getpoint "\n选取插入点:"))
	       (if (= expl "1")
	         (command "-insert" (strcat "*" dwg))
	         (command "-insert" dwg)
	       )
	       ;;(command "insert" dwg p1 strscale strscale "0")
             ;;)
	     (princ (strcat dwg "没找到!"))
	   )
	  )
    )
    (princ)
)



(defun lib_save_var ()
  (setq f (open (ZwcadSetPath "prt\\lib1.txt") "w"))
  (write-line (get_tile "ls") f)
  (write-line (itoa page) f)
  (write-line (get_tile "expl") f)
  (write-line "" f)
  (close f)
)  



(defun add_remark ()
  (new_dialog "addremark" dcl)
  (action_tile "accept" "(setq rmk (get_tile \"rmk\")) (done_dialog 1)")
  (if (and (= (start_dialog) 1) (/= rmk ""))
    (progn
      (setq rmkna (strcat (ZwcadSetPath "prt\\") icna ".txt"))
      (setq f (open rmkna "w"))
      (write-line rmk f)
      (close f)
      (set_tile "t2" rmk)
    )
  )
)



(defun extcmd_chgls ()
  (setq page 1)
  (setq typstr (nth (atoi (get_tile "ls")) lista))
  (showicon)
)



(defun libnewlist ()
  (setq	lista nil  key nil)
  (setq ff (open (ZwcadSetPath "prt\\lib.txt") "r"))
  (while (setq str (read-line ff))
    (if	(/= str "")
      (setq lista (cons str lista))
    )
  )
  (close ff)
  (setq lista (reverse lista))

  (start_list "ls")
  (mapcar 'add_list lista)
  (end_list)

  (set_tile "ls" "0")
  (setq typstr (car lista))
)

  

  
(defun showicon	()
  (mode_tile "accept" 1)
  (mode_tile "ch" 1)
  (mode_tile "zom" 1)
  (mode_tile "rem" 1)
  (mode_tile "bz" 1)
  (setq oldkeys nil)
  (set_tile "t1" (strcat "第 " (itoa page) " 页"))
  (set_tile "t2" "")
  (if (<= page 1)
    (mode_tile "prev" 1)
    (mode_tile "prev" 0)
  )
  (setq i 1)
  (repeat 16
    (setq icna (strcat typstr (itoa (+ (* (1- page) 16) i))))

    (setq sld_w (dimx_tile (strcat "icon" (itoa i))))
    (setq sld_h (dimy_tile (strcat "icon" (itoa i))))
    (start_image (strcat "icon" (itoa i)))
    (fill_image 0 0 sld_w sld_h 0)
    
    (if (findfile (strcat (ZwcadSetPath "prt\\") icna ".sld"))
      (slide_image 0 0 sld_w sld_h (strcat "prt\\" icna ".sld"))
    )

    (end_image)
    (setq i (+ i 1))
  )
)



  
(defun lib_add_ls ()
  (new_dialog "addlib" dcl)
  (action_tile "accept" "(setq libna (get_tile \"addlib\")) (done_dialog 1)")
  (if (and (= (start_dialog) 1) (/= libna ""))
    (progn
      (setq lista (reverse (cons libna (reverse lista))))
      (start_list "ls")
      (mapcar 'add_list lista)
      (end_list)

      (setq f (open (ZwcadSetPath "prt\\lib.txt") "w"))
      (setq i 0)
      (repeat (length lista)
        (write-line (nth i lista) f)
        (setq i (+ i 1))
      )
      (close f)
    )
  )
)


  
(defun lib_del_ls ()
  (if (> (length lista) 1)
    (progn
      (setq f (open (ZwcadSetPath "prt\\lib.txt") "w"))
      (setq i 0)
      (repeat (length lista)
	(if (/= (nth i lista) typstr)
	  (write-line (nth i lista) f)
	)
	(setq i (+ i 1))
      )
      (close f)
      (libnewlist)
    )
    (alert "这是唯一的图库类别,不可删除!")
  )
)


  
(defun extcmd_preview (sldname / x y)
  (new_dialog "zoom" dcl)

  (setq	x (dimx_tile "lib_zoom")
	y (dimy_tile "lib_zoom")
  )
  (start_image "lib_zoom")
  (slide_image 0 0 x y (strcat (ZwcadSetPath "prt\\") icna ".sld"))
  (end_image)
  (action_tile "accept" "(done_dialog)")
  (start_dialog)
)
  

(defun extcmd_sld_name	(iconname)
  (setq keys (atoi (substr iconname 5)))
  (if oldkeys
    (mode_tile (strcat "icon" (itoa oldkeys)) 4)
  )
  (mode_tile iconname 4)
  (setq oldkeys keys)

  (setq icna (strcat typstr (itoa (+ (* (1- page) 16) keys))))
  (if (findfile (strcat (ZwcadSetPath "prt\\") icna ".sld"))
    (progn
      (mode_tile "accept" 0)
      (mode_tile "zom" 0)
      (mode_tile "rem" 0)
      (mode_tile "bz" 0)
    )
    (progn
      (mode_tile "accept" 1)
      (mode_tile "zom" 1)
      (mode_tile "rem" 1)
      (mode_tile "bz" 1)
    )
  )
  (mode_tile "ch" 0)
  (if (findfile (strcat (ZwcadSetPath "prt\\") icna ".txt"))
    (progn
      (setq fff (open (strcat (ZwcadSetPath "prt\\") icna ".txt") "r"))
      (set_tile "t2" (read-line fff))
      (close fff)
    )
    (set_tile "t2" "")
  )
)


(defun extcmd_remove ()
  (if (findfile (strcat (ZwcadSetPath "prt\\") icna ".sld"))
    (vl-file-delete (strcat (ZwcadSetPath "prt\\") icna ".sld"))
  )
  (if (findfile (strcat (ZwcadSetPath "prt\\") icna ".dwg"))
    (vl-file-delete (strcat (ZwcadSetPath "prt\\") icna ".dwg"))
  )
  (showicon)
)

