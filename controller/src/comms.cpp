/*
void comms_handler() {

  // Timeout alarm waiting if it has been sent
  if (alarm_sent) {
    if ((Hal.millis() - alarmSentTime) >= DELAY_100MS) {
      // Alarm hasn't ack arrived within DELAY_100MS,
      // lets send the alarm again
      alarm_sent = false;
    }
  }

  switch (state) {
  case handler_state::idle:

    // TODO We don't need to execute though this FSM with the same frequency if
    // there is nothing to do

    // Do we have any incoming packets?
    if (serialIO_dataAvailable()) {
      state = handler_state::packet_arriving;
    } else if (alarm_available() &&
               !alarm_sent) { // Are any alarms waiting to be handled?
      // Change state to process alarm
      // This has lower priority than a packet recieved via serialIO
      state = handler_state::alarm_waiting;
    } else {
      state = handler_state::idle;
    }
    break;

  case handler_state::packet_arriving: // Don't know what the packet is yet
    // Keep receiving packet until completion
    received = packet_receive(rx_packet, &packet_len);

    // Check if packet has finished arriving
    if (received) {
      state = handler_state::packet_process;
    }
    break;

  case handler_state::packet_process: // Alarm ACK or Command 
    packetStatus = process_packet(rx_packet, packet_len);
    switch (packetStatus) {
    case processPacket::command:
      command_execute((enum command)rx_packet[(uint8_t)packet_field::cmd],
                      &rx_packet[(uint8_t)packet_field::data],
                      rx_packet[(uint8_t)packet_field::len], cmdResponse_data,
                      &cmdResponseData_len, sizeof(cmdResponse_data));

      // Send response to Interface Controller
      command_responseSend((uint8_t)rx_packet[(uint8_t)packet_field::cmd],
                           cmdResponse_data, cmdResponseData_len);
      break;

    case processPacket::ack:
      // We've received an alarm acknowledgement, remove the alarm from the
      // buffer
      alarm_remove();
      alarm_sent = false;
      break;

    case processPacket::nack:
      // The IC sent an ack packet (success or fail). Cannot be certain, don't
      // remove the alarm in the buffer
      alarm_sent = false;
      break;

    case processPacket::checksumErr:
      // The received packet had an invalid checksum, send error
      comms_sendChecksumERR(rx_packet);
      break;

    case processPacket::modeErr:
      // The received packet had an invalid mode, send error
      comms_sendModeERR(rx_packet);
      break;

    case processPacket::invalidErr:
      // The received packet had an invalid command, send error
      comms_sendCommandERR(rx_packet);
      break;

    case processPacket::msgTypeUnknown:
      // TODO Handle this error case
      break;

    default:
      // TODO handle this error case
      break;
    }

    state = handler_state::idle;
    break;

  case handler_state::alarm_waiting:
    // Read alarm
    send_alarm();

    alarm_sent = true;
    alarmSentTime = Hal.millis();
    state = handler_state::idle;
    break;

  default:
    // TODO Undefined state, log Error
    state = handler_state::idle; // Make sure FSM always ends in defined state
    break;
  }
}


static void send_alarm() {
  uint32_t timestamp;
  char data[ALARM_DATALEN + sizeof(timestamp)];
  enum dataID alarmID;

  if (alarm_read(&alarmID, &timestamp, &data[sizeof(timestamp)]) ==
      VC_STATUS_SUCCESS) {

    data[0] = (timestamp >> 24) & 0xFF;
    data[1] = (timestamp >> 16) & 0xFF;
    data[2] = (timestamp >> 8) & 0xFF;
    data[3] = timestamp & 0xFF;

    serialIO_send(msgType::alarm, alarmID, data, sizeof(data));
  } else {
    // TODO Handle error
  }
}


static bool packet_checksumValidation(const char *packet, uint8_t len) {
  return checksum_check(packet, len);
}

static bool packet_cmdValidation(const char *packet) {
  uint8_t cmd = (uint8_t)packet[(uint8_t)packet_field::cmd];
  return (cmd >= (uint8_t)medicalMode::start &&
          cmd <= (uint8_t)medicalMode::end) ||
         (cmd >= (uint8_t)engMode::start && cmd <= (uint8_t)engMode::end) ||
         (cmd >= (uint8_t)mixedMode::start && cmd <= (uint8_t)mixedMode::end);
}

static bool packet_modeValidation(const char *packet) {
  uint8_t cmd = (uint8_t)packet[(uint8_t)packet_field::cmd];

  // If medical mode, reject eng-mode commands.
  bool isEngMode =
      cmd >= (uint8_t)engMode::start && cmd <= (uint8_t)engMode::end;
  return parameters_getOperatingMode() != operatingMode::medical || !isEngMode;
}
*/

