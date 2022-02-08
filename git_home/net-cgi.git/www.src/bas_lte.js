function check_lte(cf)
{
	/* if(cf.username.value=="")
	{
		alert("username cannot be blank.");
		return false;
    }
    if(cf.password.value=="")
	{
		alert("password cannot be blank.");
		return false;
	} */

	if(cf.conn_mode.value != "eth_only"){	
		if(cf.apn.value.length == 0)
		{
			alert("$mobile_apn_empty");
			return false;
		}
		if(cf.apn.value.length > 64)
		{
			alert("$mobile_apn_len");
			return false;
		}
	}
    for(i=0;i<cf.username.value.length;i++)
	{
		if(isValidChar(cf.username.value.charCodeAt(i))==false)
		{
			alert("$loginname_not_allowed");
			return false;
		}
	}
	if(cf.username.value.length > 64)
	{
		alert("$user_name_error");
		return false;
	}

	for(i=0;i<cf.password.value.length;i++)
	{
		if(isValidChar(cf.password.value.charCodeAt(i))==false)
		{
			alert("$password_error");
			return false;
		}
	}
	if(cf.password.value.length > 64)
	{
		alert("$password_error");
		return false;
	}
	if(cf.pin_code.value !=0 )
	{
		var reg = new RegExp("^[0-9]{4,8}$$");
		var len = reg.test(cf.pin_code.value);  
		
		if(!len)
		{
			alert("$mobile_pin");
			return false;
		}

	}
	if(cf.puk_code.value !=0 )
	{
		var reg1 = new RegExp("^[0-9]{8}$$");
		var len1 = reg1.test(cf.puk_code.value); 
		
		if(!len1)
		{
			alert("$mobile_puk");
			return false;
		}
	}
	
	
    /* if(cf.re_conn_auto.checked == true)
	{
        cf.hid_re_conn_auto.value = "1";
    }
    else
    {
        cf.hid_re_conn_auto.value = "0";
    } */
    if(cf.roam_auto.checked == true)
	{
        cf.hid_roam_auto.value = "1";
    }
    else
    {
        cf.hid_roam_auto.value = "0";
	}
	
	
	cf.Apply.disabled = true;
	cf.submit();
	return true;
}
