
zwcadm_comps.dll: dlldata.obj zwcadm_com_p.obj zwcadm_com_i.obj
	link /dll /out:zwcadm_comps.dll /def:zwcadm_comps.def /entry:DllMain dlldata.obj zwcadm_com_p.obj zwcadm_com_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del zwcadm_comps.dll
	@del zwcadm_comps.lib
	@del zwcadm_comps.exp
	@del dlldata.obj
	@del zwcadm_com_p.obj
	@del zwcadm_com_i.obj
