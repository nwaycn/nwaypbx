# -*-coding:utf-8 -*-
__author__ = 'lihao'
import csv
from nwaycc.models import CallGroupCallout,CallGroup,CallGroupCalloutState,CallOutNumbers
from django import forms
from django.contrib import admin

class GroupCallOutForm(forms.ModelForm):
    file_to_import = forms.FileField()
    class Meta:
        model = CallGroupCallout
    def save(self, commit=False, *args, **kwargs):
        form_input = GroupCallOutForm()
        file_csv = self.cleaned_data['file_to_import']
        datafile = open(file_csv, 'rb')
        records = csv.reader(datafile)
        for line in records:
            outnumber = CallOutNumbers()
            outnumber.number = line
        datafile.close()