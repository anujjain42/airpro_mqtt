# Generated by Django 4.0.4 on 2022-09-03 10:29

from django.db import migrations, models
import django.db.models.deletion
import uuid


class Migration(migrations.Migration):

    dependencies = [
        ('broker', '0008_networkdeviceinfo_macaddr_systemdeviceinfo_macaddr_and_more'),
    ]

    operations = [
        migrations.CreateModel(
            name='ClientInfo',
            fields=[
                ('id', models.UUIDField(default=uuid.uuid4, editable=False, primary_key=True, serialize=False)),
                ('type', models.CharField(blank=True, max_length=255, null=True)),
                ('flag', models.CharField(blank=True, max_length=255, null=True)),
                ('macaddr', models.CharField(blank=True, max_length=255, null=True)),
                ('ver', models.CharField(blank=True, max_length=255, null=True)),
                ('timestamp', models.CharField(blank=True, max_length=255, null=True)),
                ('token', models.CharField(blank=True, max_length=255, null=True)),
                ('ip_address', models.GenericIPAddressField(blank=True, null=True)),
                ('status', models.CharField(blank=True, max_length=255, null=True)),
                ('request_Id', models.CharField(blank=True, max_length=255, null=True)),
                ('updated_time', models.DateTimeField(auto_now=True)),
                ('data', models.JSONField()),
                ('device_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='broker.device')),
            ],
            options={
                'abstract': False,
            },
        ),
    ]
