<?php

/*
    Copyright (C) 2003-2004 Manuel Kasper <mk@neon1.net>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
    AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

function listtags()
{
    /* The following items will be treated as arrays in config.xml */
    $ret = array(
      'acls', 'alias', 'aliasurl', 'allowedip', 'allowedhostname', 'authserver',
      'bridged', 'build_port_path',
      'ca', 'cacert', 'cert', 'crl', 'clone', 'config', 'container', 'columnitem',
<<<<<<< HEAD
      'disk', 'dnsserver', 'dnsupdate', 'domainoverrides', 'dyndns',
=======
      'disk', 'dnsserver', 'dnsupdate', 'domainoverrides',
>>>>>>> 98878a9eb90c1150b232bfbc7e9a012a3e3462a0
      'element', 'encryption-algorithm-option',
      'field', 'fieldname',
      'gateway_item', 'gateway_group', 'gif', 'gre', 'group',
      'hash-algorithm-option', 'hosts',
      'ifgroupentry', 'igmpentry', 'interface_array', 'item',
      'key',
      'lagg', 'lbpool',
      'member', 'menu', 'mobilekey', 'monitor_type', 'mount',
      'npt', 'ntpserver',
      'onetoone', 'openvpn-server', 'openvpn-client', 'openvpn-csc', 'option',
      'package', 'passthrumac', 'phase1', 'phase2', 'ppp', 'pppoe', 'priv', 'proxyarpnet', 'pool', 'pages', 'pipe',
      'radnsserver', 'roll', 'route', 'row', 'rrddatafile', 'rule',
      'schedule', 'service', 'servernat', 'servers', 'serversdisabled', 'staticmap', 'subqueue',
      'tab', 'timerange', 'tunnel',
      'user',
      'vip', 'virtual_server', 'vlan',
      'winsserver', 'wolentry', 'widget',
    );

    return array_flip($ret);
}

function startElement($parser, $name, $attrs)
{
    global $parsedcfg, $depth, $curpath, $havedata, $listtags;

    array_push($curpath, strtolower($name));

    $ptr =& $parsedcfg;
    foreach ($curpath as $path) {
        $ptr =& $ptr[$path];
    }

    /* is it an element that belongs to a list? */
    if (isset($listtags[strtolower($name)])) {
        /* is there an array already? */
        if (!is_array($ptr)) {
            /* make an array */
            $ptr = array();
        }
        array_push($curpath, count($ptr));
    } elseif (isset($ptr)) {
        /* multiple entries not allowed for this element, bail out */
        die(sprintf(
            gettext('XML error: %s at line %d cannot occur more than once') . "\n",
            $name,
            xml_get_current_line_number($parser)
        ));
    }
    $depth++;
    $havedata = $depth;
}

function endElement($parser, $name)
{
    global $depth, $curpath, $parsedcfg, $havedata, $listtags;

    if ($havedata == $depth) {
        $ptr =& $parsedcfg;
        foreach ($curpath as $path) {
            $ptr =& $ptr[$path];
        }
        $ptr = "";
    }

    array_pop($curpath);
    if (isset($listtags[strtolower($name)])) {
        array_pop($curpath);
    }

    $depth--;
}

function cData($parser, $data)
{
    global $curpath, $parsedcfg, $havedata;

    $data = trim($data, "\t\n\r");

    if ($data != "") {
        $ptr =& $parsedcfg;
        foreach ($curpath as $path) {
            $ptr =& $ptr[$path];
        }
        if (is_string($ptr)) {
            $ptr .= html_entity_decode($data);
        } else {
            if (trim($data, " ") != "") {
                $ptr = html_entity_decode($data);
                $havedata++;
            }
        }
    }
}

function parse_xml_config_raw($cffile, $rootobj, $isstring = "false")
{
    global $depth, $curpath, $parsedcfg, $havedata;
    $parsedcfg = array();
    $curpath = array();
    $depth = 0;
    $havedata = 0;

    $xml_parser = xml_parser_create();

    xml_set_element_handler($xml_parser, "startElement", "endElement");
    xml_set_character_data_handler($xml_parser, "cdata");
    xml_parser_set_option($xml_parser, XML_OPTION_SKIP_WHITE, 1);

    if (!($fp = fopen($cffile, "r"))) {
        log_msg('Error: could not open XML input', LOG_ERR);
        return -1;
    }

    while ($data = fread($fp, 4096)) {
        if (!xml_parse($xml_parser, $data, feof($fp))) {
            log_msg(sprintf(
                'XML error: %s at line %d in %s' . "\n",
                xml_error_string(xml_get_error_code($xml_parser)),
                xml_get_current_line_number($xml_parser),
                $cffile
            ), LOG_ERR);
            return -1;
        }
    }
    xml_parser_free($xml_parser);

    if ($rootobj) {
        if (!is_array($rootobj)) {
            $rootobj = array($rootobj);
        }
        foreach ($rootobj as $rootobj_name) {
            if ($parsedcfg[$rootobj_name]) {
                break;
            }
        }
        if (!$parsedcfg[$rootobj_name]) {
            log_msg(sprintf('XML error: no %s object found!' . "\n", implode(" or ", $rootobj)), LOG_ERR);
            return -1;
        }
        return $parsedcfg[$rootobj_name];
    } else {
        return $parsedcfg;
    }
}

function dump_xml_config_sub($arr, $indent)
{
    global $listtags;

    $xmlconfig = "";

    foreach ($arr as $ent => $val) {
        if (is_array($val)) {
            /* is it just a list of multiple values? */
            if (isset($listtags[strtolower($ent)])) {
                foreach ($val as $cval) {
                    if (is_array($cval)) {
                        if (empty($cval)) {
                            $xmlconfig .= str_repeat("\t", $indent);
                            $xmlconfig .= "<$ent/>\n";
                        } else {
                            $xmlconfig .= str_repeat("\t", $indent);
                            $xmlconfig .= "<$ent>\n";
                            $xmlconfig .= dump_xml_config_sub($cval, $indent + 1);
                            $xmlconfig .= str_repeat("\t", $indent);
                            $xmlconfig .= "</$ent>\n";
                        }
                    } else {
                        if ($cval === false) {
                            continue;
                        }
                        $xmlconfig .= str_repeat("\t", $indent);
                        if ((is_bool($cval) && $cval == true) || ($cval === "")) {
                            $xmlconfig .= "<$ent/>\n";
                        } elseif (
                            (substr($ent, 0, 5) == "descr")
                            || (substr($ent, 0, 6) == "detail")
                            || (substr($ent, 0, 12) == "login_banner")
                            || (substr($ent, 0, 9) == "ldap_attr")
                            || (substr($ent, 0, 9) == "ldap_bind")
                            || (substr($ent, 0, 11) == "ldap_basedn")
                            || (substr($ent, 0, 18) == "ldap_authcn")
                            || (substr($ent, 0, 19) == "ldap_extended_query")
                        ) {
                            $xmlconfig .= "<$ent><![CDATA[" . htmlentities($cval) . "]]></$ent>\n";
                        } else {
                            $xmlconfig .= "<$ent>" . htmlentities($cval) . "</$ent>\n";
                        }
                    }
                }
            } elseif (empty($val)) {
                $xmlconfig .= str_repeat("\t", $indent);
                $xmlconfig .= "<$ent/>\n";
            } else {
                /* it's an array */
                $xmlconfig .= str_repeat("\t", $indent);
                $xmlconfig .= "<$ent>\n";
                $xmlconfig .= dump_xml_config_sub($val, $indent + 1);
                $xmlconfig .= str_repeat("\t", $indent);
                $xmlconfig .= "</$ent>\n";
            }
        } else {
            if ((is_bool($val) && ($val == true)) || ($val === "")) {
                $xmlconfig .= str_repeat("\t", $indent);
                $xmlconfig .= "<$ent/>\n";
            } elseif (!is_bool($val)) {
                $xmlconfig .= str_repeat("\t", $indent);
                if (
                    (substr($ent, 0, 5) == "descr")
                    || (substr($ent, 0, 6) == "detail")
                    || (substr($ent, 0, 12) == "login_banner")
                    || (substr($ent, 0, 9) == "ldap_attr")
                    || (substr($ent, 0, 9) == "ldap_bind")
                    || (substr($ent, 0, 11) == "ldap_basedn")
                    || (substr($ent, 0, 18) == "ldap_authcn")
                    || (substr($ent, 0, 19) == "ldap_extended_query")
                ) {
                    $xmlconfig .= "<$ent><![CDATA[" . htmlentities($val) . "]]></$ent>\n";
                } else {
                    $xmlconfig .= "<$ent>" . htmlentities($val) . "</$ent>\n";
                }
            }
        }
    }

    return $xmlconfig;
}

function dump_xml_config_raw($arr, $rootobj)
{
    $xmlconfig = "<?xml version=\"1.0\"?" . ">\n";
    $xmlconfig .= "<$rootobj>\n";
    $xmlconfig .= dump_xml_config_sub($arr, 1);
    $xmlconfig .= "</$rootobj>\n";
    return $xmlconfig;
}
