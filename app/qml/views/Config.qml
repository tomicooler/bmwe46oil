import QtQuick 2.0
import com.tomi.bmwe46oil 1.0

ConfigForm {
    property Facade facade
    property DataExtractor oil

    ecuField.onTextChanged: facade.request.ecu = ecuField.text
    dataField.onTextChanged: facade.request.data = dataField.text
    updateField.onValueChanged: facade.updateInterval = updateField.value

    bytePosField.onTextChanged: oil.config.bytepos = Number(bytePosField.text)
    lengthField.onValueChanged: oil.config.length = lengthField.value
    msbSwitch.onCheckedChanged: oil.config.msb = msbSwitch.checked
    multiplierField.onTextChanged: oil.config.converter.multiplier = Number(multiplierField.text)
    offsetField.onTextChanged: oil.config.converter.offset = Number(offsetField.text)

    Component.onCompleted: {
        ecuField.text = facade.request.ecu
        dataField.text = facade.request.data
        updateField.value = facade.updateInterval

        bytePosField.text = oil.config.bytepos
        lengthField.value = oil.config.length
        msbSwitch.checked = oil.config.msb
        multiplierField.text = oil.config.converter.multiplier
        offsetField.text = oil.config.converter.offset
    }
}
