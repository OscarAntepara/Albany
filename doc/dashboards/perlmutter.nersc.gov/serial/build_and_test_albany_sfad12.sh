#!/bin/bash -login

#SBATCH -A m4274
#SBATCH --job-name=AlbanyTesting
#SBATCH --output=logs/AlbanyTesting.cpu.sfad12.%j.out
#SBATCH --error=logs/AlbanyTesting.cpu.sfad12.%j.err
#SBATCH --constraint=cpu
#SBATCH --qos=regular
#SBATCH --time=02:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=128
#SBATCH --ntasks-per-node=128
#SBATCH --ntasks-per-socket=64
#SBATCH --hint=nomultithread

BASE_DIR=/pscratch/sd/m/mcarlson/biweeklyCDashPerlmutter-serial
DEPLOY_DIR=/global/cfs/cdirs/fanssie/automated_testing/weeklyCDashPerlmutter/serial

source ${BASE_DIR}/pm_cpu_gnu_modules.sh >& ${BASE_DIR}/modules_albany.out

bash ${BASE_DIR}/nightly_cron_script_albany_pm_cpu.sh sfad 12

cp biweekly_log_pm_cpu_Albany_sfad12.txt ${DEPLOY_DIR}/logs/biweekly_log_pm_cpu_Albany_sfad12.txt

chmod -R 2770 ${DEPLOY_DIR}
chown -R :fanssie ${DEPLOY_DIR}

sbatch build_and_test_mali.sh