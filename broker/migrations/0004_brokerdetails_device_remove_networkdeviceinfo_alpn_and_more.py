# Generated by Django 4.0.1 on 2022-04-12 16:07

from django.db import migrations, models
import django.db.models.deletion
import uuid


class Migration(migrations.Migration):

    dependencies = [
        ('broker', '0003_networkdeviceinfo_flag_networkdeviceinfo_timestamp_and_more'),
    ]

    operations = [
        migrations.CreateModel(
            name='BrokerDetails',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('broker_ip', models.GenericIPAddressField()),
                ('username', models.CharField(max_length=255, unique=True)),
                ('password', models.CharField(max_length=255)),
                ('server_topic', models.CharField(max_length=255)),
            ],
        ),
        migrations.CreateModel(
            name='Device',
            fields=[
                ('id', models.UUIDField(default=uuid.uuid4, editable=False, primary_key=True, serialize=False)),
                ('device_id', models.CharField(max_length=255, unique=True)),
                ('serial_number', models.CharField(blank=True, max_length=255, null=True)),
                ('mac_address', models.CharField(blank=True, max_length=255, null=True)),
                ('alpn', models.CharField(blank=True, max_length=255, null=True)),
                ('type', models.CharField(blank=True, max_length=255, null=True)),
                ('created_on', models.DateTimeField(auto_now_add=True)),
                ('updated_on', models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.RemoveField(
            model_name='networkdeviceinfo',
            name='alpn',
        ),
        migrations.RemoveField(
            model_name='networkdeviceinfo',
            name='mac_address',
        ),
        migrations.RemoveField(
            model_name='networkdeviceinfo',
            name='serial_number',
        ),
        migrations.RemoveField(
            model_name='systemdeviceinfo',
            name='alpn',
        ),
        migrations.RemoveField(
            model_name='systemdeviceinfo',
            name='mac_address',
        ),
        migrations.RemoveField(
            model_name='systemdeviceinfo',
            name='serial_number',
        ),
        migrations.RemoveField(
            model_name='wifideviceinfo',
            name='alpn',
        ),
        migrations.RemoveField(
            model_name='wifideviceinfo',
            name='mac_address',
        ),
        migrations.RemoveField(
            model_name='wifideviceinfo',
            name='serial_number',
        ),
        migrations.AlterField(
            model_name='networkdeviceinfo',
            name='device_id',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='broker.device'),
        ),
        migrations.AlterField(
            model_name='systemdeviceinfo',
            name='device_id',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='broker.device'),
        ),
        migrations.AlterField(
            model_name='wifideviceinfo',
            name='device_id',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='broker.device'),
        ),
    ]
