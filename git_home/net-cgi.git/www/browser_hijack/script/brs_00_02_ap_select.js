function initPage(){var c=document.getElementsByTagName("h1");var b=document.createTextNode(bh_ap_select_title);c[0].appendChild(b);var g=document.getElementsByTagName("p");var f=document.createTextNode(bh_ap_select_doc);g[0].appendChild(f);var e=document.getElementById("btnsContainer_div");e.onclick=function(){return genieHelpChecking()};var d=document.getElementById("btn_div");var a=document.createTextNode(bh_next_mark);d.appendChild(a);showFirmVersion("none")}function genieHelpChecking(){var b=document.getElementById("choices_div");var d=b.getElementsByTagName("input");var a=document.getElementsByTagName("form");var c=a[0];if(d[0].checked){location.href="BRS_01_checkNet_ping.html"}else{if(d[1].checked){location.href="BRS_00_03_ap_setup.html"}else{alert(bh_select_an_option);return false}}return true}function goto_url(){window.open("ap_learn.html","show_statistic")}addLoadEvent(initPage);