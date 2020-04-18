#include "serdes.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

bool serdes_decode_incomming_packet(uint8_t *rx_buffer, uint8_t rx_buffer_len,
                                    size_t encoded_len,
                                    GuiAckHandler_t gui_ack_handler,
                                    CommandHandler_t command_handler) {
  pb_istream_t stream = pb_istream_from_buffer(rx_buffer, encoded_len);
  Packet packet = Packet_init_zero;
  bool status = pb_decode(&stream, Packet_fields, &packet);
  if (status) {
    if (Packet_cmd_tag == packet.which_payload) {
      command_handler(packet.payload.cmd);
    } else if (Packet_gui_ack_tag == packet.which_payload) {
      gui_ack_handler(packet.payload.gui_ack);
    } else {
      return false;
    }
  }

  return status;
}

bool serdes_encode_status_packet(ControllerStatus controller_status,
                                 uint8_t *tx_buffer, uint8_t tx_buffer_len,
                                 size_t *encoded_len) {
  pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, tx_buffer_len);


  ControllerData cdp = ControllerData_init_zero;
  cdp.msg_type = ControllerMsgType_DATA;
  cdp.response_to_cmd = CommandType_NONE;
  cdp.which_payload = ControllerData_status_tag;
  cdp.payload.status = controller_status;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_data_tag;
  packet.payload.data = cdp;

  bool status = pb_encode(&stream, Packet_fields, &packet);

  *encoded_len = stream.bytes_written;

  return status;
}

uint8_t about_buffer[64];

bool ControllerIdentification_callback(pb_istream_t *istream,
                                       pb_ostream_t *ostream,
                                       const pb_field_t *field) {
  if (ostream && field->tag == ControllerIdentification_about_tag) {
    /* Our custom data type is char* */
    const char *str = *(const char **)field->pData;

    if (!pb_encode_tag_for_field(ostream, field))
      return false;

    return pb_encode_string(ostream, (const uint8_t *)str, strlen(str));
  }

  if (istream && field->tag == ControllerIdentification_about_tag) {

    uint32_t strlen = istream->bytes_left;

    if (strlen > sizeof(about_buffer) - 1)
      return false;

    about_buffer[strlen] = '\0';

    if (!pb_read(istream, about_buffer, strlen))
      return false;
    (*(ControllerIdentification *)(field->message)).about =
        (char *)about_buffer;
  }

  return true;
}

bool serdes_encode_controllerid_packet(ControllerIdentification cid,
                                       uint8_t *tx_buffer,
                                       uint8_t tx_buffer_len,
                                       size_t *encoded_len) {
  pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer, tx_buffer_len);

  ControllerData cdp = ControllerData_init_zero;
  cdp.msg_type = ControllerMsgType_STATUS;
  cdp.response_to_cmd = CommandType_NONE;
  cdp.which_payload = ControllerData_identification_tag;
  cdp.payload.identification = cid;

  Packet packet = Packet_init_zero;
  packet.which_payload = Packet_data_tag;
  packet.payload.data = cdp;

  bool status = pb_encode(&stream, Packet_fields, &packet);
  *encoded_len = stream.bytes_written;

  return status;
}
