{% set isEnabled=[] %}
{% if helpers.exists('Reticen8.captiveportal.zones.zone') %}
{%   for cpZone in  helpers.toList('Reticen8.captiveportal.zones.zone') %}
{%     if cpZone.enabled|default('0') == '1' %}
{%	do isEnabled.append(cpZone) %}
{%     endif %}
{%   endfor %}
{% endif
%}
captiveportal_defer="YES"
captiveportal_enable="{% if isEnabled %}YES{% else %}NO{% endif %}"
