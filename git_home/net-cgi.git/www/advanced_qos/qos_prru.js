function gray_rule_table(){var e=document.forms[0];var d=getElementsByTagAndName("div","queue_list");document.getElementById("qos_rules").style.color="black";document.getElementById("qos_show_rule").style.color="black";for(var b=0;b<d.length;b++){c_nodes=d[b].childNodes;for(var a=0;a<c_nodes.length;a++){c_nodes[a].className="grey_DragBox"}}}function trun_bandwidth(){var a=document.forms[0];setDisabled(false,a.tcbw_value,a.tcbw_unit)}function show_fasttrack(){var a=document.forms[0];change_qos_type();document.getElementById("qos_fast_status").style.display="none";document.getElementById("qos_tru_table").style.display="none";document.getElementById("qos_fast_onoff").style.display="none";document.getElementById("blue_line_3").style.display="none";document.getElementById("qos_trust").style.display="none";document.getElementById("qos_band").style.display="none";document.getElementById("qos_wps").style.display="none";document.getElementById("qos_wps1").style.display="none";document.getElementById("qos_wps2").style.display="none"}function change_qos_mode(){var a=document.forms[0];a.hid_qos_mode.value="0";document.getElementById("qos_select").style.display="";click_fasttrack=0;show_fasttrack();document.getElementsByName("help_iframe")[0].src="/help/_qos_help.html"}function setGray(){var a=document.forms[0];trun_bandwidth();change_qos_mode();gray_rule_table();a.qos_endis_on.value="1";document.getElementById("qos_select").style.color="black";setDisabled(false,a.qos_type[0],a.qos_type[1]);if(a.hid_qos_type.value=="0"){document.getElementById("qos_app").style.color="black";document.getElementById("qos_ser_policy").style.color="black";document.getElementById("qos_ser_pri").style.color="black";document.getElementById("qos_port_type").style.color="black";document.getElementById("qos_port_start").style.color="black";document.getElementById("qos_port_end").style.color="black";a.qos_apps.disabled=false;a.ser_poli_name.disabled=false;a.ser_priority.disabled=false;a.port_type.disabled=false;a.portstart.disabled=false;a.portend.disabled=false}else{if(a.hid_qos_type.value=="1"){document.getElementById("qos_device").style.color="black";document.getElementById("qos_dev_table").style.color="black";document.getElementById("qos_dev_policy").style.color="black";document.getElementById("qos_dev_mac").style.color="black";document.getElementById("qos_dev_name").style.color="black";document.getElementById("qos_dev_pri").style.color="black";a.dev_poli_name.disabled=false;a.mac.disabled=false;a.dev_name.disabled=false;a.dev_priority.disabled=false}}if(master=="admin"){a.Add.disabled=false;a.Add.className="new_apply_bt ui-corner-all";a.Reset.disabled=false;a.Reset.className="new_apply_bt2 ui-corner-all";a.Edit.disabled=false;a.Edit.className="new_apply_bt ui-corner-all";a.Deleteone.disabled=false;a.Deleteone.className="new_apply_bt ui-corner-all";a.DeleteAll.disabled=false;a.DeleteAll.className="new_apply_bt2 ui-corner-all"}}function show_other(){var a=document.forms[0];if((parent.is_pr_version==1&&a.qos_apps.selectedIndex==40)||(parent.is_ru_version==1&&a.qos_apps.selectedIndex==33)||(parent.is_pr_version==0&&parent.is_ru_version==0&&a.qos_apps.selectedIndex==31)){document.getElementById("qos_ser_policy").style.display="";document.getElementById("qos_port_type").style.display="";document.getElementById("qos_port_start").style.display="";document.getElementById("qos_port_end").style.display="";return true}else{document.getElementById("qos_ser_policy").style.display="none";document.getElementById("qos_port_type").style.display="none";document.getElementById("qos_port_start").style.display="none";document.getElementById("qos_port_end").style.display="none";return false}}function change_qos_type(){var a=document.forms[0];if(a.qos_type[0].checked==true){a.hid_qos_type.value="0";show_other();document.getElementById("qos_app").style.display="";document.getElementById("qos_ser_pri").style.display="";document.getElementById("qos_device").style.display="none";document.getElementById("qos_dev_table").style.display="none";document.getElementById("blue_line_2").style.display="none";document.getElementById("qos_dev_policy").style.display="none";document.getElementById("qos_dev_mac").style.display="none";document.getElementById("qos_dev_name").style.display="none";document.getElementById("qos_dev_pri").style.display="none"}else{if(a.qos_type[1].checked==true){a.hid_qos_type.value="1";document.getElementById("qos_app").style.display="none";document.getElementById("qos_ser_policy").style.display="none";document.getElementById("qos_port_type").style.display="none";document.getElementById("qos_port_start").style.display="none";document.getElementById("qos_port_end").style.display="none";document.getElementById("qos_ser_pri").style.display="none";document.getElementById("qos_device").style.display="";document.getElementById("qos_dev_table").style.display="";document.getElementById("blue_line_2").style.display="";document.getElementById("qos_dev_policy").style.display="";document.getElementById("qos_dev_mac").style.display="";document.getElementById("qos_dev_name").style.display="";document.getElementById("qos_dev_pri").style.display=""}}document.getElementById("blue_line_1").style.display="";document.getElementById("qos_button_1").style.display="";document.getElementById("qos_button_2").style.display="";document.getElementById("qos_rules").style.display="";document.getElementById("qos_show_rule").style.display="";document.getElementById("blue_line_4").style.display=""}function change_serv_apps(){var b=document.forms[0];var a=b.qos_apps.selectedIndex;show_other();if(parent.is_pr_version==1){b.qos_apps.options.length=41}else{if(parent.is_ru_version==1){b.qos_apps.options.length=34}else{b.qos_apps.options.length=32}}}function change_service_name_show(a){if(a=="IP_Phone"){new_name="$qos_ipphone"}else{if(a=="Skype"){new_name="$qos_skype"}else{if(a=="Netgear_EVA"){new_name="$qos_netgear"}else{if(a=="Vonage_IP_Phone"){new_name="$qos_vonage"}else{if(a=="Google_Talk"){new_name="$qos_google"}else{if(a=="MSN_messenger"){new_name="$qos_msn"}else{if(a=="Yahoo_messenger"){new_name="$qos_yahoo"}else{if(a=="Netmeeting"){new_name="$qos_netmeeting"}else{if(a=="AIM"){new_name="$qos_aim"}else{if(a=="SlindStream"){new_name="$qos_slingstream"}else{if(a=="SSH"){new_name="$block_ser_setup_ssh"}else{if(a=="Telnet"){new_name="$block_ser_setup_telnet"}else{if(a=="VPN"){new_name="$qos_vpn"}else{if(a=="On_line_Game"){new_name="On-line Game"}else{if(a=="FTP"){new_name="$ftp_mark"}else{if(a=="SMTP"){new_name="$block_ser_setup_smtp"}else{if(a=="PPlive"){new_name="$qos_pplive"}else{if(a=="WWW"){new_name="$qos_www"}else{if(a=="DNS"){new_name="$block_ser_setup_dns"}else{if(a=="ICMP"){new_name="$qos_icmp"}else{if(a=="eMule"){new_name="$qos_emule"}else{if(a=="Kazaa"){new_name="$qos_kazaa"}else{if(a=="Gnutella"){new_name="$qos_gnutella"}else{if(a=="bt_azureus"){new_name="$qos_bt_azureus"}else{if(a=="Counter-Strike"){new_name="$qos_counter_strike"}else{if(a=="Age-of-Empires"){new_name="$qos_ageof_empires"}else{if(a=="Everquest"){new_name="$qos_everquest"}else{if(a=="Quake-2"){new_name="$qos_quake2"}else{if(a=="Quake-3"){new_name="$qos_quake3"}else{if(a=="Unreal-Tourment"){new_name="$qos_unreal"}else{if(a=="Warcraft"){new_name="$qos_warcraft"}else{if(a=="Thunder"){new_name="$qos_pr_thunder"}else{if(a=="QQ_xuanfeng"){new_name="$qos_pr_qq_xuanfeng"}else{if(a=="LOL"){new_name="$qos_pr_lol"}else{if(a=="YY_yuyin"){new_name="$qos_pr_yy"}else{if(a=="Tonghuashun"){new_name="$qos_pr_tonghuashun"}else{if(a=="Dazhihui"){new_name="$qos_pr_dazhihui"}else{if(a=="sipnet.ru"){new_name="$qos_ru_sipnet"}else{new_name=a}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}return new_name}function attached_qosmac_data_selected(a){var b=document.forms[0];b.dev_poli_name.value=attach_policy[a];if(attach_mac_array[a]=="&lt;$unknown_mark&gt;"){b.mac.value=""}else{b.mac.value=attach_mac_array[a]}if(attach_name_array[a]=="&lt;$unknown_mark&gt;"){b.dev_name.value=""}else{b.dev_name.value=attach_name_array[a].replace(/&gt;/g,">").replace(/&lt;/g,"<").replace(/&#38;/g,"&").replace(/&#35;/g,"#").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#40;/g,"(").replace(/&#41;/g,")")}b.dev_priority.value="2"}function attached_qosmac_selected(a){var b=document.forms[0];if(attach_mac_array[a]=="&lt;$unknown_mark&gt;"){b.trust_mac.value=""}else{b.trust_mac.value=attach_mac_array[a]}}function check_qos_port(a){if(a.portstart.value==""||a.portend.value==""){sAlert("$invalid_port");return false}txt=a.portstart.value;for(i=0;i<txt.length;i++){c=txt.charAt(i);if("0123456789".indexOf(c,0)<0){sAlert("$invalid_start_port");return false}}txt=a.portend.value;for(i=0;i<txt.length;i++){c=txt.charAt(i);if("0123456789".indexOf(c,0)<0){sAlert("$invalid_end_port");return false}}if(parseInt(a.portstart.value)<1||parseInt(a.portstart.value)>65535){sAlert("$invalid_start_port");return false}if(parseInt(a.portend.value)<1||parseInt(a.portend.value)>65535){sAlert("$invalid_end_port");return false}if(parseInt(a.portend.value)<parseInt(a.portstart.value)){sAlert("$end_port_greater");return false}return true}function q_ser_queue(){var a=document.forms[0];if(a.ser_priority.value=="0"){q_index=0}else{if(a.ser_priority.value=="1"){q_index=1}else{if(a.ser_priority.value=="2"){q_index=2}else{if(a.ser_priority.value=="3"){q_index=3}}}}return q_index}function q_dev_queue(){var a=document.forms[0];if(a.dev_priority.value=="0"){q_index=0}else{if(a.dev_priority.value=="1"){q_index=1}else{if(a.dev_priority.value=="2"){q_index=2}else{if(a.dev_priority.value=="3"){q_index=3}}}}return q_index}function add_rules(){var b=document.forms[0];var a=getElementsByTagAndName("div","queue_list");if(b.qos_type[0].checked==true){q_ser_queue();change_serv_apps();index=b.qos_apps.selectedIndex;o_value=b.qos_apps[index].value;o_app_type=b.qos_apps[index].value;if((parent.is_pr_version==1&&index==40)||(parent.is_ru_version==1&&index==33)||(parent.is_pr_version==0&&parent.is_ru_version==0&&index==31)){if(!check_qos_add_info()){return false}o_value=b.ser_poli_name.value.replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");o_app_type="1";o_app_conn=b.port_type.value;o_app_start=b.portstart.value;o_app_end=b.portend.value;remove_ser_from_queue(o_value,o_app_type);newitem=creat_other_new_item(o_value,o_app_type,o_app_conn,o_app_start,o_app_end);a[q_index].appendChild(newitem)}else{remove_ser_from_queue(o_value,o_app_type);newitem=creat_a_new_item(o_value,o_app_type);a[q_index].appendChild(newitem)}}else{q_dev_queue();if(!check_qosMac_info(b,"add")){return false}o_value=b.dev_poli_name.value;o_app_type="3";o_dev_name=b.dev_name.value;o_dev_mac=b.the_mac.value;newitem=creat_a_dev_new_item(o_value,o_app_type,o_dev_name,o_dev_mac);a[q_index].appendChild(newitem)}update_qos_rules();b.submit_flag.value="add_qoslist";b.action="/apply.cgi?/QOS_basic_setting.htm timestamp="+ts;return true}function reset_rules(){var a=document.forms[0];if(a.qos_type[0].checked==true){if(show_other()){a.ser_poli_name.value="";a.ser_priority.value="1";a.port_type.value="TCP/UDP";a.portstart.value="";a.portend.value=""}else{a.qos_apps.value="MSN_messenger";a.ser_priority.value="1"}}else{a.dev_poli_name.value="";a.mac.value="";a.dev_name.value="";a.dev_priority.value="2"}}function edit_qos_rules(){var cf=document.forms[0];var queue_lists=getElementsByTagAndName("div","queue_list");var array_num=qos_array_num/2;if(selected_node!=0){var node_value=selected_node.getAttribute("val0_name").replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var node_app_type=selected_node.getAttribute("val2_app_name");for(var i=1;i<=qos_array_num;i=i+2){var str=eval("qosArray"+i).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var each_info=str.split(" ");if(node_value==each_info[0]){cf.select_node_editnum.value=i;if(node_app_type=="3"){cf.action="/apply.cgi?/QOS_device_edit.htm timestamp="+ts}else{cf.select_node_name.value=node_value;cf.action="/apply.cgi?/QOS_service_edit.htm timestamp="+ts}cf.submit_flag.value="qos_editnode";return true}}}else{sAlert("$port_edit");return false}}function delete_qos_rule(){var b=document.forms[0];var a=getElementsByTagAndName("div","queue_list");if(selected_node!=0){if(selected_node.parentNode==a[0]){a[0].removeChild(selected_node)}else{if(selected_node.parentNode==a[1]){a[1].removeChild(selected_node)}else{if(selected_node.parentNode==a[2]){a[2].removeChild(selected_node)}else{if(selected_node.parentNode==a[3]){a[3].removeChild(selected_node)}}}}update_qos_rules();b.submit_flag.value="qos_deletenode";b.action="/apply.cgi?/QOS_wait.htm timestamp="+ts;return true}else{sAlert("$port_del");return false}}function clear_qos_rules(){var f=document.forms[0];var e=getElementsByTagAndName("div","queue_list");for(var b=0;b<4;b++){var d=e[b];var a=d.childNodes.length;while(d.childNodes.length>0){d.removeChild(d.childNodes[0])}}update_qos_rules();f.submit_flag.value="qos_deletenode";f.action="/apply.cgi?/QOS_wait.htm timestamp="+ts}function create_one_rule(d,b){var a=document.forms[0];b=b.replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");one_rule=document.createElement("input");one_rule.type="hidden";one_rule.name="hid_qos_list"+d;one_rule.value=""+b+"";a.appendChild(one_rule)}function find_qos_item_details(o_value,l_name,l_num,p_value,compare_type,num){cf=document.forms[0];for(var x=1;x<=l_num;x=x+2){var str=eval(l_name+x);var each_info=str.split(" ");if(o_value==each_info[compare_type]){num++;var i_value=each_info[0];for(var t=1;t<each_info.length;t++){if(t==3){i_value=i_value+" "+p_value}else{i_value=i_value+" "+each_info[t]}}create_one_rule(num,i_value);num++;var str2=eval(l_name+(x+1));if(str2=="0"){i_value="0"}else{each_info=str2.split(" ");i_value=each_info[0];for(t=1;t<each_info.length;t++){if(t==3){i_value=i_value+" "+p_value}else{i_value=i_value+" "+each_info[t]}}}create_one_rule(num,i_value);break}}return num}function creat_other_qos_item_details(e,f,a,h,b,g,k,d){cf=document.forms[0];var j=e;if(j!=""){d++;if(b=="TCP/UDP"){j=e+" "+f+" "+a+" "+h+" TCP "+g+" "+k+" ---- ----";create_one_rule(d,j);d++;j=e+" "+f+" "+a+" "+h+" UDP "+g+" "+k+" ---- ----";create_one_rule(d,j)}else{j=e+" "+f+" "+a+" "+h+" "+b+" "+g+" "+k+" ---- ----";create_one_rule(d,j);d++;j="0";create_one_rule(d,j)}}return d}function creat_a_dev_qos_item_details(g,d,e,f,b){cf=document.forms[0];var a=g;if(a!=""){b++;a=g+" 3 ---- "+d+" ---- ---- ---- "+e+" "+f;create_one_rule(b,a);b++;create_one_rule(b,a)}return b}function update_qos_rules(){var d=document.forms[0];var e=0;if(d.qos_endis_on.value=="1"){queue_lists=getElementsByTagAndName("div","queue_list");for(var b=0;b<4;b++){qlist=queue_lists[b];for(var a=0;a<qlist.childNodes.length;a++){childnode=qlist.childNodes[a];node_value=childnode.getAttribute("val0_name");node_app_type=childnode.getAttribute("val2_app_name");if(node_app_type=="1"){node_app_conn=childnode.getAttribute("val3_app_conn");node_app_start=childnode.getAttribute("val4_app_start");node_app_end=childnode.getAttribute("val5_app_end");c_n=creat_other_qos_item_details(node_value,0,1,b,node_app_conn,node_app_start,node_app_end,e)}else{if(node_app_type=="3"){node_dev_name=childnode.getAttribute("val3_dev_name");node_dev_mac=childnode.getAttribute("val4_mac");c_n=creat_a_dev_qos_item_details(node_value,b,node_dev_name,node_dev_mac,e)}else{c_n=find_qos_item_details(node_app_type,"qosArray",qos_array_num,b,2,e)}}if(c_n==e){c_n=find_qos_item_details(node_value,"qosDftArray",qos_dft_array_num,b,2,e)}e=c_n}}}}function check_prru_qos_apply(a){if(a.qos_fast.checked==true){a.hid_fast_status.value="1"}else{a.hid_fast_status.value="0"}if(a.wps_fastlane[0].checked==true){a.hid_wps_fastlen.value="wps"}else{a.hid_wps_fastlen.value="fastlane"}if(a.hid_wps_fastlen.value=="fastlane"&&w_f_value=="wps"){sAlert("$wps_to_fasttrack",check_prru_qos_apply2())}else{check_prru_qos_apply2()}}function check_prru_qos_apply2(){var a=document.forms[0];qos_bandwith=parseInt(a.tcbw_value.value);if(max_bandwidth==1000){if(a.tcbw_unit.selectedIndex==0){if(!(qos_bandwith>0&&qos_bandwith<1024001)){sAlert("$qos_bandwith1000M");return false}else{a.qos_hidden_uprate.value=parseInt(a.tcbw_value.value)}}else{if(!(qos_bandwith>0&&qos_bandwith<1001)){sAlert("$qos_bandwith1000M");return false}else{a.qos_hidden_uprate.value=parseInt(a.tcbw_value.value)*1024}}}else{if(max_bandwidth==100){if(a.tcbw_unit.selectedIndex==0){if(!(qos_bandwith>0&&qos_bandwith<102401)){sAlert("$qos_bandwith100M");return false}else{a.qos_hidden_uprate.value=parseInt(a.tcbw_value.value)}}else{if(!(qos_bandwith>0&&qos_bandwith<101)){sAlert("$qos_bandwith100M");return false}else{a.qos_hidden_uprate.value=parseInt(a.tcbw_value.value)*1024}}}}a.submit()}function check_name_space(b){var d=["MSN Messenger","Yahoo Messenger","IP Phone","Vonage IP Phone","Google Talk","Netgear EVA","On-line Game","LAN Port 1","LAN Port 2","LAN Port 3","LAN Port 4","Counter Strike","Age of Empires","Diablo II","Half Life","Quake 2","Quake 3","Unreal Tourment","Return to Castle Wolfenstein"];var a=0;for(i=0;i<d.length;i++){if(b==d[i]){a=1}}if(a==0){for(i=0;i<b.length;i++){if(isValidChar(b.charCodeAt(i))==false){sAlert("$qos_policy $not_allow_2bytes_space");return false}}}}function check_qos_add_info(){var cf=document.forms[0];var index=cf.qos_apps.selectedIndex;if(cf.ser_poli_name.value==""){sAlert("$qos_policy_name_null");return false}if(check_name_space(cf.ser_poli_name.value)==false){return false}if((parent.is_pr_version==1&&index==40)||(parent.is_ru_version==1&&index==33)||(parent.is_pr_version==0&&parent.is_ru_version==0&&index==31)){if(check_qos_port(cf)==false){return false}}if((parent.is_pr_version==1&&index==40)||(parent.is_ru_version==1&&index==33)||(parent.is_pr_version==0&&parent.is_ru_version==0&&index==31)){var input_start_port=cf.portstart.value;var input_end_port=cf.portend.value}for(i=1;i<=qos_array_num;){var str=eval("qosArray"+i).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var each_info=str.split(" ");var startport=each_info[5];var endport=each_info[6];var j=i+1;var str_next=eval("qosArray"+j).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");if(str_next!="0"){each_info[4]="TCP/UDP"}if(cf.ser_poli_name.value==change_service_name_show(each_info[0])){sAlert("$qos_policy_name_dup");return false}if((parent.is_pr_version==1&&index==40)||(parent.is_ru_version==1&&index==33)||(parent.is_pr_version==0&&parent.is_ru_version==0&&index==31)){if(cf.port_type.value=="TCP/UDP"||each_info[4]=="TCP/UDP"||cf.port_type.value==each_info[4]){if(each_info[5]!="----"&&each_info[6]!="----"){if(!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10))){sAlert("$qos_port_used");return false}}}}i=i+2}return true}function check_qos_app(cf){var cf=document.forms[0];var select_editnum=selected_node;var str_edit=eval("qosArray"+select_editnum).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var str_info=str_edit.split(" ");var startport=str_info[5];var endport=str_info[6];if(str_info[2]=="1"){if(cf.ser_poli_name.value==""){sAlert("$qos_policy_name_null");return false}if(check_name_space(cf.ser_poli_name.value)==false){return false}if(check_qos_port(cf)==false){return false}}for(var i=1;i<=qos_array_num;i=i+2){var str=eval("qosArray"+i).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var each_info=str.split(" ");var startport=each_info[5];var endport=each_info[6];var j=i+1;var str_next=eval("qosArray"+j).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");if(str_next!="0"){each_info[4]="TCP/UDP"}if(str_info[2]=="1"){var input_start_port=cf.portstart.value;var input_end_port=cf.portend.value;if(cf.ser_poli_name.value==change_service_name_show(each_info[0])&&select_editnum!=i){sAlert("$qos_policy_name_dup");return false}if(cf.port_type.value=="TCP/UDP"||each_info[4]=="TCP/UDP"||cf.port_type.value==each_info[4]){if(each_info[5]!="----"&&each_info[6]!="----"){if((!(parseInt(endport,10)<parseInt(input_start_port,10)||parseInt(input_end_port,10)<parseInt(startport,10)))&&select_editnum!=i){sAlert("$qos_port_used");return false}}}}if(select_editnum==i){if(each_info[2]=="1"){creat_other_qos_item_details(cf.ser_poli_name.value,each_info[1],each_info[2],cf.ser_priority.value,cf.port_type.value,cf.portstart.value,cf.portend.value,i-1)}else{find_qos_item_details(each_info[2],"qosArray",qos_array_num,cf.ser_priority.value,2,i-1)}}}}function check_qosMac_info(cf,flag){var cf=document.forms[0];var select_editnum=selected_node;if(cf.dev_name.value==""){sAlert("$device_name_null");return false}if(check_name_space(cf.dev_name.value)==false){return false}for(i=0;i<cf.dev_name.value.length;i++){if(isValidChar(cf.dev_name.value.charCodeAt(i))==false){sAlert("$device_name_error");cf.dev_name.focus();return false}}cf.the_mac.value=cf.mac.value;var mac_addr=cf.the_mac.value;if(mac_addr.indexOf(":")==-1&&mac_addr.length=="12"){var tmp_mac=mac_addr.substr(0,2)+":"+mac_addr.substr(2,2)+":"+mac_addr.substr(4,2)+":"+mac_addr.substr(6,2)+":"+mac_addr.substr(8,2)+":"+mac_addr.substr(10,2);mac_addr=tmp_mac}cf.the_mac.value=mac_addr;if(maccheck(cf.the_mac.value)==false){return false}for(i=1;i<=qos_array_num;){var str=eval("qosArray"+i).replace(/&#92;/g,"\\").replace(/&lt;/g,"<").replace(/&gt;/g,">").replace(/&#40;/g,"(").replace(/&#41;/g,")").replace(/&#34;/g,'"').replace(/&#39;/g,"'").replace(/&#35;/g,"#").replace(/&#38;/g,"&");var each_info=str.split(" ");if((flag=="edit"&&select_editnum!=i)||flag=="add"){if(cf.dev_name.value==each_info[7]){sAlert("$device_name_dup");return false}if(cf.dev_poli_name.value==each_info[0]){sAlert("$qos_policy_name_dup");return false}if(cf.the_mac.value.toLowerCase()==each_info[8].toLowerCase()){sAlert("$mac_dup");return false}}i=i+2}var def_name=cf.the_mac.value.substring(9,11)+cf.the_mac.value.substring(12,14)+cf.the_mac.value.substring(15,17);if(cf.dev_poli_name.value==""){cf.dev_poli_name.value="Pri_MAC_"+def_name}if(check_name_space(cf.dev_poli_name.value)==false){return false}for(i=0;i<cf.dev_poli_name.value.length;i++){if(isValidChar(cf.dev_poli_name.value.charCodeAt(i))==false){sAlert("$qos_policy_deny_2bytes");cf.dev_poli_name.focus();return false}}return true}function check_qos_mac(d,a){var d=document.forms[0];var b=qos_array_num/2;if(!check_qosMac_info(d,a)){return false}i_value=d.dev_poli_name.value+" 3 ---- "+d.dev_priority.value+" ---- ---- ---- "+d.dev_name.value+" "+d.the_mac.value;create_one_rule(selected_node,i_value);selected_node++;create_one_rule(selected_node,i_value)};