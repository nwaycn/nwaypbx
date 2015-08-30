#-*-coding:utf-8-*-
from django.contrib import admin
from nwaycc.models import *
from nwaycc.forms import GroupCallOutForm
class CallAfterOptAdmin(admin.ModelAdmin):
    list_display = ( 'id', 'name')

class CDRAdmin(admin.ModelAdmin):
    list_display = ('id','caller_id_name','caller_id_number', 'destination_number', 'start_stamp', 'a_answer_stamp', 'a_end_stamp', 'duration', 'mduration', 'recording_file', 'hangup_cause', 'call_gateway', 'called_number')
    search_fields = ('caller_id_number', 'destination_number')
    #fields = (, )
    #禁用添加修改
    #def has_add_permission(self, request):
    #    return False
    #def has_change_permission(self, request, obj=None):
    #    return False
    readonly_fields = ('id','caller_id_name','caller_id_number', 'destination_number', 'start_stamp', 'a_answer_stamp', 'a_end_stamp', 'duration', 'recording_file', 'hangup_cause', 'call_gateway', 'called_number')


class ConcurrTypeAdmin(admin.ModelAdmin):
    list_display = ('id', 'concurr_type_name')
    fields = ('id', 'concurr_type_name')
    readonly_fields = ('id', )

class DialplanDetailAdmin(admin.ModelAdmin):
    list_display = ('id', 'dialplan', 'dialplan_detail_tag', 'dialplan_detail_type_id', 'dialplan_detail_data', 'dialplan_detail_break', 'dialplan_detail_inline', 'dialplan_detail_group', 'dialplan_detail_order')
    fields = ( 'id', 'dialplan', 'dialplan_detail_tag', 'dialplan_detail_type_id', 'ring_id', 'dialplan_detail_data', 'dialplan_detail_break', 'dialplan_detail_inline', 'dialplan_detail_group', 'dialplan_detail_order')
    search_fields = ('id',  'dialplan', 'dialplan_detail_tag', 'dialplan_detail_inline', 'dialplan_detail_group', 'dialplan_detail_order')
    readonly_fields = ('id', )

class DialplanAdmin(admin.ModelAdmin):
    list_display = ('id', 'dialplan_name', 'dialplan_context', 'dialplan_number', 'dialplan_continue', 'dialplan_order', 'dialplan_enabled')
    search_fields = ('dialplan_name', 'dialplan_enabled', )
    fields = ('id', 'dialplan_name', 'dialplan_context', 'dialplan_number', 'dialplan_continue', 'dialplan_order', 'dialplan_enabled', 'dialplan_description')
    readonly_fields = ('id', )

class ExtensionAdmin(admin.ModelAdmin):
    list_display = ('id', 'extension_name', 'extension_number', 'is_allow_callout', 'callout_number', 'callout_gateway', 'extension_type', 'group', 'extension_login_state', 'extension_reg_state')
    search_fields = ('extension_name', 'callout_number', 'callout_gateway')
    fields = ('id', 'extension_name', 'extension_number', 'is_allow_callout', 'is_record', 'callout_number', 'callout_gateway', 'extension_type', 'group', 'extension_pswd', 'extension_login_state', 'extension_reg_state')
    readonly_fields = ('id', )

class ExtensionGroupAdmin(admin.ModelAdmin):
    list_display = ('id', 'group_name', 'group_description')
    search_fields = ('id', 'group_name' )
    fields = ('id', 'group_name', 'group_description')
    readonly_fields = ('id', )

class ExtensionTypeAdmin(admin.ModelAdmin):
    list_display = ('id', 'type_name')
    search_fields = ('id', 'type_name')
    fields = ('id', 'type_name')
    readonly_fields = ('id', )

class FunctionAdmin(admin.ModelAdmin):
    list_display = ('id', 'function_name')
    search_fields = ('id', 'function_name')
    fields = ('id', 'function_name', 'function_description')
    readonly_fields = ('id', )

class GateWayAdmin(admin.ModelAdmin):
    list_display = ('id', 'gateway_name', 'gateway_url', 'call_prefix', 'max_call')
    search_fields = ('id', 'gateway_name')
    fields = ('id', 'gateway_name', 'gateway_url', 'call_prefix', 'max_call')
    #readonly_fields = ('id', )

class CallGroupAdmin(admin.ModelAdmin):
    list_display = ('id', 'group_name' )
    search_fields = ('id', 'group_name')
    fields = ('id', 'group_name')
    #readonly_fields = ('id', )

class CallGroupCalloutAdmin(admin.ModelAdmin):
    #form = GroupCallOutForm
    list_display = ('id', 'callout_name', 'number_group', 'number_group_uploadfile', 'callout_state', 'run_position', \
                    'start_time', 'stop_time', 'total_number', 'wait_number', 'success_number', 'failed_number', \
                    'cancel_number' )
    #fields = ('id', 'callout_name', 'number_group', 'number_group_uploadfile', 'run_position', 'start_time', \
    #          'stop_time', 'total_number', 'wait_number', 'success_number', 'failed_number', 'cancel_number', \
    #          'callout_state', 'concurr_type', 'concurr_number',  'group', 'time_rule', 'ring', 'after_ring_play',\
    #         'ring_timeout')
    search_fields = ('callout_name', 'callout_state')
    #form = GroupCallOutForm
    readonly_fields = ('id', 'total_number', 'wait_number', 'success_number', 'failed_number', 'cancel_number', \
        'run_position')

class CallGroupCalloutStateAdmin(admin.ModelAdmin):
    list_display = ('id', 'state_name' )
    search_fields = ('id', 'state_name')
    fields = ('id', 'state_name')
    #readonly_fields = ('id', )

class HangupCauseAdmin(admin.ModelAdmin):
    list_display = ('id', 'hangup_cause', 'hangup_cause_desc')
    search_fields = ('id', 'hangup_cause')
    fields = ('id', 'hangup_cause', 'hangup_cause_desc')
    #readonly_fields = ('id', )

class IvrMenuOptionsAdmin(admin.ModelAdmin):
    list_display = ('id', 'ivr_menu', 'ivr_menu_option_digits', 'ivr_menu_option_action_id', 'ivr_menu_option_param', \
        'ivr_menu_option_order', 'ivr_menu_option_description')
    search_fields = ('id', 'ivr_menu_option_digits', 'ivr_menu')
    fields = ('id', 'ivr_menu', 'ivr_menu_option_digits', 'ivr_menu_option_action_id', 'ivr_menu_option_param', \
        'ivr_menu_option_order', 'ivr_menu_option_description', 'ring_id')
    #readonly_fields = ('id', )

class IvrMenusAdmin(admin.ModelAdmin):
    list_display = ('id', 'ivr_menu_name', 'ivr_menu_extension', 'ivr_menu_greet_long_id', 'ivr_menu_greet_short_id',\
        'ivr_menu_invalid_sound_id', 'ivr_menu_exit_sound_id', 'ivr_menu_confirm_macro', 'ivr_menu_confirm_key', \
        'ivr_menu_confirm_attempts', 'ivr_menu_timeout', 'ivr_menu_exit_app_id', 'ivr_menu_enabled')
    search_fields = ('id', 'ivr_menu_name', 'ivr_menu_extension')
    fields = ('id', 'ivr_menu_name', 'ivr_menu_extension', 'ivr_menu_greet_long_id', 'ivr_menu_greet_short_id',\
        'ivr_menu_invalid_sound_id', 'ivr_menu_exit_sound_id',  'ivr_menu_confirm_macro', 'ivr_menu_confirm_key', \
        'ivr_menu_confirm_attempts', 'ivr_menu_timeout', 'ivr_menu_exit_app_id', 'ivr_menu_exit_data', \
        'ivr_menu_inter_digit_timeout', 'ivr_menu_max_failures', 'ivr_menu_max_timeouts', 'ivr_menu_digit_len', \
        'ivr_menu_direct_dial', 'ivr_menu_ringback_id', 'ivr_menu_enabled', 'ivr_menu_call_order_id', 'ivr_menu_call_crycle_order',\
        'ivr_menu_description')
    #readonly_fields = ('id', )
class CallOperationAdmin(admin.ModelAdmin):
    list_display = ('id', 'name', 'description')
    fields = ('id', 'name', 'description')

class CallOrderAdmin(admin.ModelAdmin):
    list_display = ('id', 'order_name', 'order_description' )
    search_fields = ('id', 'order_name')
    fields = ('id', 'order_name', 'order_description')
    #readonly_fields = ('id', )

class CallOutNumbersAdmin(admin.ModelAdmin):
    list_display = ('id', 'group', 'number', 'is_called', 'call_state', 'start_time', 'answer_time', 'hangup_time', 'hangup_reason',\
        'answer_extension', 'record_file', 'wait_sec')
    search_fields = ('id', 'group', 'number')
    fields = ('id', 'group', 'number', 'is_called', 'call_state', 'start_time', 'answer_time', 'hangup_time', 'hangup_reason',\
        'answer_extension', 'record_file', 'wait_sec')
    #readonly_fields = ('id', )

class CallOutsideConfigAdmin(admin.ModelAdmin):
    list_display = ('id', 'outside_line_name', 'outside_line_number', 'inside_line_number', 'is_record', 'is_voice_mail', \
        'call_order', 'call_crycle_order')
    search_fields = ('id', 'outside_line_name', 'outside_line_number')
    fields = ('id', 'outside_line_name', 'outside_line_number', 'inside_line_number', 'is_record', 'is_voice_mail', \
        'call_order', 'call_crycle_order')
    #readonly_fields = ('id', )

class RingsAdmin(admin.ModelAdmin):
    list_display = ('id', 'ring_name', 'ring_path')
    search_fields = ('ring_name',)
    fields = ('id',  'ring_name', 'ring_path')
    #readonly_fields = ('id', )

class CallTimePlanAdmin(admin.ModelAdmin):
    list_display = ('id',  'name', 'plan_timing', 'plan_date', 'per_hour', 'per_day', 'per_month', 'is_monday', 'is_tuesday',\
        'is_wednesday', 'is_thursday', 'is_friday', 'is_saturday', 'is_sunday')
    search_fields = ('name',)
    fields = ('id',  'name', 'plan_timing', 'plan_date', 'per_hour', 'per_day', 'per_month', 'is_monday', 'is_tuesday',\
        'is_wednesday', 'is_thursday', 'is_friday', 'is_saturday', 'is_sunday')
    #readonly_fields = ('id', )

class InOutMappingAdmin(admin.ModelAdmin):
    list_display = ('id',  'outside_line', 'inside_line', 'order_number')
    search_fields = ('id', 'outside_line', 'inside_line', 'order_number')
    fields = ('id',  'outside_line', 'inside_line', 'order_number')
    #readonly_fields = ('id', )

class CalloutGatewaysAdmin(admin.ModelAdmin):
    fields = ('id', 'name', 'gateway')

admin.site.register(CallCdr, CDRAdmin)
#admin.site.register(CallConcurrType, ConcurrTypeAdmin)
admin.site.register(CallDialplanDetails, DialplanDetailAdmin)
admin.site.register(CallDialplans, DialplanAdmin)
admin.site.register(CallExtension, ExtensionAdmin)
admin.site.register(CallExtensionGroups, ExtensionGroupAdmin)
#admin.site.register(CallExtensionType,ExtensionTypeAdmin)
#admin.site.register(CallFunctions, FunctionAdmin)
admin.site.register(CallGateway, GateWayAdmin)
admin.site.register(CallGroup, CallGroupAdmin)
#admin.site.register(CallGroupCalloutState, CallGroupCalloutStateAdmin)
admin.site.register(CallGroupCallout ,CallGroupCalloutAdmin)
#admin.site.register(CallHangupCause , HangupCauseAdmin)
admin.site.register(CallIvrMenuOptions, IvrMenuOptionsAdmin)
admin.site.register(CallIvrMenus, IvrMenusAdmin)
#admin.site.register(CallOrder, CallOrderAdmin)
admin.site.register(CallOutNumbers, CallOutNumbersAdmin)
admin.site.register(CallOutsideConfig, CallOutsideConfigAdmin)
admin.site.register(CallRings,RingsAdmin)
#admin.site.register(CallTimePlan, CallTimePlanAdmin)
#admin.site.register(CallVoicemail)
admin.site.register(InOutMapping, InOutMappingAdmin)
admin.site.register(NumberParagraph)
#admin.site.register(CallOperation, CallOperationAdmin)
#admin.site.register(CallAfterOpt, CallAfterOptAdmin)
admin.site.register(CalloutGateways, CalloutGatewaysAdmin)
# Register your models here.
