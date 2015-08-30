# coding=utf-8
# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Remove `managed = False` lines if you wish to allow Django to create and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
#
# Also note: You'll have to insert the output of 'django-admin.py sqlcustom [appname]'
# into your database.
from __future__ import unicode_literals

from django.db import models
from django.db import connection
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

class string_with_title(str):
    def __new__(cls, value, title):
        instance = str.__new__(cls, value)
        instance._title = title
        return instance

    def title(self):
        return self._title

    __copy__ = lambda self: self
    __deepcopy__ = lambda self, memodict: self
def get_sequence_val(seq_name):
    cursor = connection.cursor()
    cursor.execute('select nextval(\'%s\')' % seq_name)
    maxvalue = 1
    try:
        maxvalue = cursor.fetchone()[0]
    except:
        maxvalue = 1
    return maxvalue

class CallAfterOpt(models.Model):


    id = models.IntegerField(primary_key=True, default=0)

   # id = models.AutoField(primary_key=True) #='call_afteropt_id_seq') #models.IntegerField(primary_key=True)

    name = models.CharField('名称', max_length=50)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_afteropt_id_seq')
        super(CallAfterOpt,self).save( *args, **kwargs)
    @classmethod
    def create(cls, id):
        return cls(id=get_sequence_val('call_afteropt_id_seq'))
    class Meta:
        #app_label = string_with_title('Nwaycc', '呼叫中心' )
        #app_label = 'call center'
        managed = False
        verbose_name = '执行操作'
        verbose_name_plural = '执行操作'
        db_table = 'call_after_opt'

    def __unicode__(self):
        return self.name


class CallCdr(models.Model):
    id = models.BigIntegerField('序号',primary_key=True, default=0)
    accountcode = models.TextField('计费帐号',blank=True)
    xml_cdr = models.TextField('xmlcdr',blank=True)
    caller_id_name = models.CharField('主叫姓名',max_length=50, blank=True)
    caller_id_number = models.CharField('主叫号码',max_length=50, blank=True)
    destination_number = models.CharField('被叫号码',max_length=50, blank=True)
    star_epoch = models.DecimalField('保留字段',max_digits=65535, decimal_places=65535, blank=True, null=True)
    start_stamp = models.DateTimeField('开始时间',blank=True, null=True)
    a_answer_stamp = models.DateTimeField('a应答时间',blank=True, null=True)
    a_answer_epoch = models.DecimalField('保留字段',max_digits=65535, decimal_places=65535, blank=True, null=True)
    a_end_epoch = models.IntegerField('保留字段',   blank=True,   default=0)#('',max_digits=65535, decimal_places=65535, blank=True, null=True)
    a_end_stamp = models.DateTimeField('a挂机时间',blank=True, null=True)

    b_answer_stamp = models.DateTimeField('b应答时间',blank=True, null=True)
    b_answer_epoch = models.DecimalField('保留字段',max_digits=65535, decimal_places=65535, blank=True, null=True)
    b_end_epoch = models.IntegerField('保留字段',   blank=True,   default=0)#('',max_digits=65535, decimal_places=65535, blank=True, null=True)
    b_end_stamp = models.DateTimeField('b挂机时间',blank=True, null=True)

    duration = models.IntegerField('a通话时长',   blank=True,   default=0)#DecimalField('主叫通话时长',max_digits=65535, decimal_places=65535, blank=True, null=True)
    mduration = models.IntegerField('b通话时长',   blank=True,   default=0)#DecimalField('通话时长',max_digits=65535, decimal_places=65535, blank=True, null=True)
    billsec = models.IntegerField('通话',   blank=True,   default=0)#DecimalField('通话',max_digits=65535, decimal_places=65535, blank=True, null=True)
    recording_file = models.CharField('录音文件',max_length=255, blank=True)
    conference_name = models.CharField('保留字段',max_length=50, blank=True)
    conference_id = models.BigIntegerField('保留字段',blank=True, null=True)
    digites_dialed = models.CharField('保留字段',max_length=50, blank=True)
    hangup_cause = models.CharField('挂机原因',max_length=200, blank=True)
    hangup_cause_0 = models.ForeignKey('CallHangupCause', db_column='hangup_cause_id', blank=True, null=True) # Field renamed because of name conflict.
    waitsec = models.IntegerField(blank=True, null=True)
    call_gateway = models.ForeignKey('CallGateway', blank=True, null=True)
    called_number = models.BigIntegerField('来电外线',blank=True, null=True )
    class Meta:
        managed = False
        db_table = 'call_cdr'
        verbose_name = '通话清单'
        verbose_name_plural = '通话清单'
        #app_label = '历史清单'
    def __unicode__(self):
        return u'%s %s', self.caller_id_number, self.destination_number

class CallOperation(models.Model):
    id = models.IntegerField(primary_key=True,  default=0)
    name = models.CharField(max_length=255, blank=True)
    description = models.CharField(max_length=255, blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_operation_id_seq')
        super(CallOperation,self).save( *args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_operation'
        verbose_name = '拨号计划操作'
        verbose_name_plural = '拨号计划操作'
        #app_label = '基本配置'
    def __unicode__(self):
        return self.name

class CallConcurrType(models.Model):
    id =  models.BigIntegerField(primary_key=True,  default=0)
    concurr_type_name = models.CharField('外呼类型名称',max_length=255)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_concurr_type_id_seq')
        super(CallConcurrType,self).save( *args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_concurr_type'
        verbose_name = '外呼类型'
        verbose_name_plural = '外呼类型'
    def __unicode__(self):
        return self.concurr_type_name

class CallDialplanDetails(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #'') #models.BigIntegerField(primary_key=True, default=0)
    dialplan = models.ForeignKey('CallDialplans', blank=True, null=True)
    dialplan_detail_tag = models.CharField('标记', max_length=255, blank=True)
    dialplan_detail_type_id = models.ForeignKey('CallOperation', db_column='dialplan_detail_type_id' ,blank=True, null=True, help_text='指定相应的下级操作')
    #dialplan_detail_type = models.TextField('类型', blank=True)
    ring_id = models.ForeignKey('CallRings', db_column='ring_id', blank=True, null=True, help_text='只在播放彩铃操作时指定有效')
    dialplan_detail_data = models.TextField('数据', blank=True, help_text='拨号规则中的如:user/$1@{domain_name}, user/$1等perl的正则表达式,移动^1(3[4-9]|5[012789]|8[78])\\d{8};联通^1(3[0-2]|5[56]|8[56])\\d{8}; \
     电信^18[09]\\d{8};CDMA^1[35]3\\d{8};')
    dialplan_detail_break = models.BooleanField('是否中止', blank=True, help_text='当遇到这个节点后，如果有异常或不满足条件，是否中止呼叫')
    dialplan_detail_inline = models.TextField('内线', blank=True)
    dialplan_detail_group = models.ForeignKey('CallExtensionGroups', blank=True, null=True, help_text='分机组,用于分机组的拨号规则')
    dialplan_detail_order =  models.IntegerField('序号',   blank=True,   default=0)
     #models.ForeignKey('CallOrder', blank=True, null=True, help_text='')
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_dialplan_details_id_seq')
        super(CallDialplanDetails,self).save( *args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_dialplan_details'
        verbose_name = '拨号规则明细'
        verbose_name_plural = '拨号规则明细'
    def __unicode__(self):
        return self.dialplan_detail_tag

class CallDialplans(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='') #models.BigIntegerField(primary_key=True, default=0)
    dialplan_name = models.CharField('名称', max_length=255, blank=True)
    dialplan_context = models.CharField('内容', max_length=255, blank=True)
    dialplan_number = models.CharField('号码', max_length=255, blank=True , help_text='perl正则表达式:^10[01][0-9]$ , ^*78#$,正则表达式,移动^1(3[4-9]|5[012789]|8[78])\\d{8};联通^1(3[0-2]|5[56]|8[56])\\d{8}; \
     电信^18[09]\\d{8};CDMA^1[35]3\\d{8};')
    dialplan_continue = models.BooleanField('是否继续', default=False)
    dialplan_order = models.IntegerField('序号',   blank=True,   default=0)
    dialplan_enabled = models.BooleanField('启用', default=True) #models.CharField('启用',max_length=255, blank=True)
    dialplan_description = models.TextField('说明', blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_dialplans_id_seq')
        super(CallDialplans,self).save( *args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_dialplans'
        verbose_name = '拨号规则'
        verbose_name_plural = '拨号规则'
    def __unicode__(self):
        return self.dialplan_name

class CallExtension(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_extension_id_seq') #models.BigIntegerField(primary_key=True, default=0)
    extension_name = models.CharField('分机名称', max_length=50)
    extension_number = models.CharField('分机号码', max_length=50)
    is_allow_callout = models.BooleanField('允许外呼', default= True)
    is_record = models.BooleanField('是否录音', default= False)
    callout_number = models.CharField('外呼号码', max_length=50, blank=True)
    callout_gateway = models.ForeignKey('CallGateway',  db_column='callout_gateway', blank=True, null=True, help_text='外呼网关')  #CallGateway
    extension_type = models.ForeignKey('CallExtensionType', db_column='extension_type', blank=True, null=True, help_text='分机类型')
    group = models.ForeignKey('CallExtensionGroups', blank=True, null=True, help_text='所属坐席组')
    extension_pswd = models.CharField('分机密码', max_length=130, blank=True)
    extension_login_state = models.CharField('登录状态', max_length=50, blank=True)
    extension_reg_state = models.CharField('注册状态', max_length=50, blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_extension_id_seq')
        super(CallExtension, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_extension'
        verbose_name = '分机管理'
        verbose_name_plural = '分机管理'
    def __unicode__(self):
        return  self.extension_number

class CallExtensionGroups(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_extension_groups_id_seq') #models.BigIntegerField(primary_key=True, default=0)
    group_name = models.CharField('分机组名', max_length=255)
    group_description = models.CharField('分机分组说明', max_length=500, blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_extension_groups_id_seq')
        super(CallExtensionGroups, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_extension_groups'
        verbose_name = '分机组管理'
        verbose_name_plural = '分机组管理'
    def __unicode__(self):
        return self.group_name

class CallExtensionType(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_extension_type_id_seq') #models.BigIntegerField(primary_key=True, default=0)
    type_name = models.CharField('分机分组名称', max_length=50, blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_extension_type_id_seq')
        super(CallExtensionType, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_extension_type'
        verbose_name = '分机类型'
        verbose_name_plural = '分机类型'
    def __unicode__(self):
        return self.type_name

class CallFunctions(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_functions_id_seq') #models.BigIntegerField(primary_key=True, default=0)
    function_name = models.CharField('电话功能名称', max_length=255, blank=True)
    function_description = models.TextField('电话功能说明', blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_functions_id_seq')
        super(CallFunctions, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_functions'
        verbose_name = '功能管理'
        verbose_name_plural = '功能管理'
    def __unicode__(self):
        return self.function_name

class CallGateway(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_gateway_id_seq') # models.BigIntegerField(primary_key=True, default=0)
    gateway_name = models.CharField('网关名称', max_length=255, blank=True)
    gateway_url = models.CharField('网关url', max_length=255, blank=True, help_text='如：sofia/gateway/gw1')
    call_prefix = models.CharField('呼叫前缀', max_length=50, blank=True, help_text='如: 9*')
    max_call = models.IntegerField('允许并发数', blank=True, null=True, help_text='最大并发数', default=10)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_gateway_id_seq')
        super(CallGateway, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_gateway'
        verbose_name = '网关管理'
        verbose_name_plural = '网关管理'
    def __unicode__(self):
        return self.gateway_name

class CallGroup(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='call_group_id_seq') #models.BigIntegerField(primary_key=True, default=0)
    group_name = models.CharField('群呼组名', max_length=50)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_group_id_seq')
        super(CallGroup, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_group'
        verbose_name = '群呼号码组'
        verbose_name_plural = '群呼号码分组'
    def __unicode__(self):
        return self.group_name

class CallGroupCallout(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='')#models.BigIntegerField(primary_key=True,default=0)
    callout_name = models.CharField('群呼名称', max_length=255, blank=True)
    callout_state = models.ForeignKey('CallGroupCalloutState', blank=True,  help_text='呼叫状态')
    number_group = models.ForeignKey('CallGroup',  blank=True,  help_text='呼叫号码组')  #db_column='number_group_id',
    number_group_uploadfile = models.FileField(u'文件', upload_to='uploads/groupfiles/%Y/%m/%d', help_text='只接受.csv为扩展名的表格文件，首列填入相应的号码如010-87655665或13671922577')
    run_position = models.BigIntegerField('当前序号', blank=True, null=True, default=0)
    callout_gateway = models.ForeignKey('CalloutGateways', blank=True,  help_text='外呼网关')
    concurr_type = models.ForeignKey('CallConcurrType', blank=True,   help_text='外呼的类型,如预测外呼,精准外呼,群呼') #_id

    start_time = models.DateTimeField('开始时间', blank=True, null=True)
    stop_time = models.DateTimeField('停止时间', blank=True, null=True)
    ring = models.ForeignKey('CallRings', db_column='ring_id', blank=True, null=True, help_text='彩铃', related_name='ring')
    after_ring_play = models.ForeignKey(CallAfterOpt, db_column='after_ring_play',help_text='播放彩铃后', blank=True, related_name='after_ring_play')

    second_ring_id = models.ForeignKey('CallRings', db_column='second_ring_id', blank=True, null=True, help_text='二次彩铃,上一个选项为播放彩铃有效', related_name='second_ring')
    second_after_ring_opt = models.ForeignKey(CallAfterOpt, db_column='second_after_ring_opt', help_text='二次播放后的操作', blank=True, null=True, related_name='second_after_ring_opt')
    after_ring_key = models.CharField('按键', max_length=255, blank=True, default='#')
    after_key_opt_id = models.ForeignKey(CallAfterOpt, db_column='after_key_opt_id', blank=True, null=True, help_text='按键后操作', related_name='after_key_opt_id')
    ring_timeout = models.IntegerField('播放彩铃超时', blank=True, null=True, default=30)
    group = models.ForeignKey('CallExtensionGroups',  blank=True,  help_text='要呼转的坐席组') #
    call_project_id = models.BigIntegerField(blank=True, null=True, default=0)

    max_concurr_number = models.IntegerField('最大并发数', blank=True, null=True, default=10)
    concurr_number = models.IntegerField('并发倍数',   blank=True, null=True, default=1)
    time_rule = models.ForeignKey('CallTimePlan', blank=True, null=True, help_text='定时计划')
    total_number = models.IntegerField('号码总数', blank=True, null=True, default=0)
    wait_number = models.IntegerField('等待号码总数', blank=True, null=True, default=0)
    success_number = models.IntegerField('接通数量', blank=True, null=True, default=0)
    failed_number = models.IntegerField('失败数量', blank=True, null=True, default=0)
    cancel_number = models.IntegerField('取消数量', blank=True, null=True, default=0)
    outside_line_number = models.CharField('外显号码', max_length=255, blank=True , help_text='外显号码，用于主动呼叫出去时，显示到手机或来电显示固话上的号码，必须是运营商许可的')
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_group_callout_id_seq')
        super(CallGroupCallout, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_group_callout'
        verbose_name = '群呼管理'
        verbose_name_plural = '群呼管理'
    def __unicode__(self):
        return self.callout_name

class CallGroupCalloutState(models.Model):
    id = models.IntegerField(primary_key=True, default=0)
    state_name = models.CharField('状态名称', max_length=255)
    class Meta:
        managed = False
        db_table = 'call_group_callout_state'
        verbose_name = '群呼呼叫状态'
        verbose_name_plural = '群呼呼叫状态'
    def __unicode__(self):
        return self.state_name

class CallHangupCause(models.Model):
    id = models.BigIntegerField(primary_key=True, default=0)
    hangup_cause = models.CharField('挂机理由名称', max_length=200, blank=True)
    hangup_cause_desc = models.TextField('挂机理由说明', blank=True)
    class Meta:
        managed = False
        db_table = 'call_hangup_cause'
        verbose_name = '挂机原因'
        verbose_name_plural = '挂机原因'
    def __unicode__(self):
        return self.hangup_cause

class CallIvrMenuOptions(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='') #models.BigIntegerField(primary_key=True, default=0)
    ivr_menu = models.ForeignKey('CallIvrMenus', blank=True, null=True)
    ivr_menu_option_digits = models.CharField('IVR按键', max_length=50, blank=True)
    #ivr_menu_option_action = models.CharField('IVR动作', max_length=255, blank=True)
    ivr_menu_option_param = models.CharField('IVR参数', max_length=1000, blank=True)
    ivr_menu_option_order = models.IntegerField('IVR序号', blank=True, null=True)
    ivr_menu_option_description = models.TextField('IVR参数说明', blank=True)
    ring_id = models.ForeignKey('CallRings', db_column='ring_id', blank=True, null=True)
    ivr_menu_option_action_id = models.ForeignKey('CallOperation',db_column='ivr_menu_option_action_id', blank=True, null=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_ivr_menu_options_id_seq')
        super(CallIvrMenuOptions, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_ivr_menu_options'
        verbose_name = 'IVR明细'
        verbose_name_plural = 'IVR明细'
    def __unicode__(self):
        return self.ivr_menu_option_digits

class CallIvrMenus(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='') #models.BigIntegerField(primary_key=True, default=0)
    ivr_menu_name = models.CharField('IVR名称', max_length=200, blank=True)
    ivr_menu_extension = models.TextField('IVR分机', blank=True)

    ivr_menu_call_order_id = models.ForeignKey('CallOrder', db_column='ivr_menu_call_order_id', blank=True, null=True, help_text='呼叫排序')
    ivr_menu_greet_long_id = models.ForeignKey('CallRings', db_column='ivr_menu_greet_long_id', blank=True, null=True, help_text='长铃声', related_name='ivr_menu_greet_long_id')
    ivr_menu_greet_short_id = models.ForeignKey('CallRings', db_column='ivr_menu_greet_short_id', blank=True, null=True, help_text='短铃声', related_name='ivr_menu_greet_short_id')
    ivr_menu_invalid_sound_id = models.ForeignKey('CallRings',db_column='ivr_menu_invalid_sound_id',  blank=True, null=True, help_text='无效彩铃', related_name='ivr_menu_invalid_sound_id')
    ivr_menu_exit_sound_id = models.ForeignKey('CallRings',db_column='ivr_menu_exit_sound_id',  blank=True, null=True, help_text='退出彩铃', related_name='ivr_menu_exit_sound_id')
    ivr_menu_ringback_id = models.ForeignKey('CallRings',db_column='ivr_menu_ringback_id', blank=True, null=True, help_text='背景音乐', related_name='ivr_menu_ringback_id')
    ivr_menu_exit_app_id = models.ForeignKey(CallOperation,db_column='ivr_menu_exit_app_id', blank=True, null=True)

    ivr_menu_confirm_macro = models.CharField('IVR确认宏', max_length=200, blank=True)
    ivr_menu_confirm_key = models.CharField('IVR确认键', max_length=200, blank=True, default='#') #如#号结束
    ivr_menu_confirm_attempts = models.IntegerField('IVR尝试次数', blank=True, null=True, default=3)
    ivr_menu_timeout = models.IntegerField('IVR超时(秒)', blank=True, null=True, default=60)
   # ivr_menu_exit_app = models.CharField('IVR退出App', max_length=200, blank=True)
    ivr_menu_exit_data = models.TextField('IVR退出参数', blank=True)
    ivr_menu_inter_digit_timeout = models.IntegerField('IVR按键超时', blank=True, null=True, default=30)
    ivr_menu_max_failures = models.IntegerField('IVR最大尝试次数', blank=True, null=True)
    ivr_menu_max_timeouts = models.IntegerField('IVR最大超时', blank=True, null=True, default=60)
    ivr_menu_digit_len = models.IntegerField('IVR按键长度', blank=True, null=True, default=5)
    ivr_menu_direct_dial = models.CharField('IVR直转号码', max_length=200, blank=True)

    ivr_menu_cid_prefix = models.CharField('IVR的CID前缀', max_length=200, blank=True)
    ivr_menu_enabled = models.BooleanField('启用',  blank=True, default=True)
    ivr_menu_description = models.TextField('说明', blank=True)
    #ivr_menu_call_order = models.ForeignKey('CallOrder', blank=True, null=True)
    ivr_menu_call_crycle_order = models.IntegerField('循环呼位置', blank=True, null=True, default= 0)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_ivr_menus_id_seq')
        super(CallIvrMenus, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_ivr_menus'
        verbose_name = 'IVR管理'
        verbose_name_plural = 'IVR管理'
    def __unicode__(self):
        return self.ivr_menu_name

#class CallOperation(models.Model):
#    id = models.IntegerField(primary_key=True, default=0)
#    name = models.CharField(max_length=255, blank=True)
#    description = models.CharField(max_length=255, blank=True)
 #   class Meta:
 #       managed = False
 #       db_table = 'call_operation'
 #   def __unicode__(self):
 #       return self.name

class CallOrder(models.Model):
    id = models.BigIntegerField(primary_key=True, default=0)
    order_name = models.CharField('呼叫顺序名称', max_length=255, blank=True)
    order_description = models.TextField('呼叫顺序说明', blank=True)
    class Meta:
        managed = False
        db_table = 'call_order'
        verbose_name = '呼叫顺序'
        verbose_name_plural = '呼叫顺序'
    def __unicode__(self):
        return self.order_name

class CallOutNumbers(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='') #models.BigIntegerField(primary_key=True, default=0)
    group = models.ForeignKey('CallGroup', blank=True, null=True) # db_column='group_id',
    number = models.CharField('号码', max_length=50, blank=True)
    is_called = models.BooleanField('是否呼叫过', default=False)
    call_state = models.IntegerField('呼叫结事', blank=True, null=True)
    start_time = models.DateTimeField('开始时间', blank=True, null=True)
    answer_time = models.DateTimeField('应答时间', blank=True, null=True)
    hangup_time = models.DateTimeField('挂机时间', blank=True, null=True)
    hangup_reason = models.ForeignKey(CallHangupCause, blank=True, null=True)
    answer_extension = models.ForeignKey(CallExtension, blank=True, null=True)
    record_file = models.CharField('录音文件', max_length=255, blank=True)
    wait_sec = models.IntegerField('等待时间', blank=True, null=True, default=0)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_out_numbers_id_seq')
        super(CallOutNumbers, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_out_numbers'
        verbose_name = '批量外呼号码'
        verbose_name_plural = '批量外呼号码'
    def __unicode__(self):
        return self.number

class CallOutsideConfig(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
     #='') #models.BigIntegerField(primary_key=True, default=0)
    outside_line_name = models.CharField('外线名称', max_length=50, blank=True)
    outside_line_number = models.CharField('外线号码', max_length=50, blank=True)
    inside_line_number = models.CharField('内线号码', max_length=50, blank=True)
    is_record = models.BooleanField('启用录音', default=False)
    is_voice_mail = models.BooleanField('启用语音信箱', default=False)
    call_order = models.ForeignKey(CallOrder, blank=True, null=True)
    call_crycle_order = models.BigIntegerField('循环呼叫序号', blank=True, null=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_outside_config_id_seq')
        super(CallOutsideConfig, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_outside_config'
        verbose_name = '外线配置'
        verbose_name_plural = '外线配置'
    def __unicode__(self):
        return self.outside_line_number

class CallRings(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='') #models.BigIntegerField(primary_key=True, default=0)
    ring_name = models.CharField('彩铃名称',max_length=200, blank=True)
    ring_path = models.FileField(u'文件', upload_to='uploads/rings')
    ring_description = models.TextField('彩铃说明',blank=True)
    ring_category = models.BigIntegerField('彩铃类型',blank=True, null=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_rings_id_seq')
        super(CallRings, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_rings'
        verbose_name = '彩铃管理'
        verbose_name_plural = '彩铃管理'
    def __unicode__(self):
        return self.ring_name

class CallTimePlan(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='') #models.BigIntegerField(primary_key=True, default=0)
    name = models.CharField('名称', max_length=255)
    plan_timing = models.BooleanField('一次性定时', default=False)
    plan_date = models.DateField('定时日期', blank=True, null=True)
    per_hour = models.NullBooleanField('每小时', default=False)
    per_day = models.NullBooleanField('每天', default=False)
    per_month = models.NullBooleanField('每月', default=False)
    is_monday = models.NullBooleanField('周一', default=False)
    is_tuesday = models.NullBooleanField('周二', default=False)
    is_wednesday = models.NullBooleanField('周三', default=False)
    is_thursday = models.NullBooleanField('周四', default=False)
    is_friday = models.NullBooleanField('周五', default=False)
    is_saturday = models.NullBooleanField('周六', default=False)
    is_sunday = models.NullBooleanField('周日', default=False)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_time_plan_id_seq')
        super(CallTimePlan, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_time_plan'
        verbose_name = '定时管理'
        verbose_name_plural = '定时管理'
    def __unicode__(self):
        return self.name

class CallVoicemail(models.Model):
    id = models.BigIntegerField(primary_key=True, default=0)
    extension = models.ForeignKey(CallExtension)
    voicemail_password = models.CharField('语音信箱密码', max_length=50, blank=True)
    greeting_id = models.BigIntegerField(blank=True, null=True)
    voicemail_mail_to = models.CharField(max_length=50, blank=True)
    voicemail_attach_file = models.CharField(max_length=255, blank=True)
    voicemail_local_after_email = models.CharField(max_length=255, blank=True)
    voicemail_enabled = models.CharField(max_length=50, blank=True)
    voicemail_desc = models.TextField(blank=True)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('call_time_plan_id_seq')
        super(CallVoicemail, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'call_voicemail'
        verbose_name = '语音信箱'
        verbose_name_plural = '语音信箱'
    def __unicode__(self):
        return self.extension

class CalloutGateways(models.Model):
    id = models.IntegerField(primary_key=True, default=0)
    name = models.CharField(max_length=255)
    gateway = models.ForeignKey(CallGateway, blank=True, null=True)
    class Meta:
        managed = False
        db_table = 'callout_gateways'
        verbose_name = '外呼网关'
        verbose_name_plural = '外呼网关'
    def __unicode__(self):
        return self.name

class InOutMapping(models.Model):
    id = models.BigIntegerField(primary_key=True,  default=0)
    #='') #models.BigIntegerField(primary_key=True, default=0)
    outside_line = models.ForeignKey(CallOutsideConfig, blank=True, null=True)
    inside_line = models.ForeignKey(CallExtension, blank=True, null=True)
    order_number = models.IntegerField('序号', blank=True, null=True, default=0)
    def save(self, *args, **kwargs):
        if (self.id < 1):
            self.id = get_sequence_val('in_out_mapping_id_seq')
        super(InOutMapping, self).save(*args, **kwargs)
    class Meta:
        managed = False
        db_table = 'in_out_mapping'
        verbose_name = '内外线对应管理'
        verbose_name_plural = '内外线对应管理'
    def __unicode__(self):
        return u'%s %s', self.outside_line, self.inside_line

class NumberParagraph(models.Model):
    number_paragraph = models.CharField(max_length=50, primary_key=True)
    class Meta:
        managed = False
        db_table = 'number_paragraph'
        verbose_name = '本地号段'
        verbose_name_plural = '本地号段'
    def __unicode__(self):
        return self.number_paragraph

